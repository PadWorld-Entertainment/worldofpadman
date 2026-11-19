#!/usr/bin/env python3
"""Test WoP's shipped map/mode combinations with bots and an optional spectator client (Python 3.9+)."""

import argparse
from contextlib import ExitStack
import json
import os
from pathlib import Path
import re
import socket
import subprocess
import tempfile
import time


ROOT = Path(__file__).resolve().parent.parent
ARENAS = (
    "scripts.pk3dir/scripts/wop_maps.arena",
    "padpack.pk3dir/scripts/wop_padpack.arena",
    "mappack.pk3dir/scripts/wop_mappack.arena",
)
MAP_PACKS = ("maps.pk3dir", "padpack.pk3dir", "mappack.pk3dir")
MODES = {
    "ffa": 0, "syc_ffa": 3, "lps": 4, "ctkd": 5, "team": 6,
    "ft": 7, "ctl": 8, "1lc": 9, "syc_tp": 10, "bb": 11,
}
BOT_MODELS = ("Padman", "PadGirl", "PadLilly", "PadRock", "StonePad", "PiratePad", "MonsterPad", "PadCho")
NUM_BOTS = 40
SPECTATOR = "MatrixSpectator"
TEAM_SPECTATOR = "3"
PMF_FOLLOW = 4096
TIME_EXIT = "Timelimit hit."
POINT_EXIT = "Pointlimit hit."
ROUND_EXIT = "FreezeTag round ended."
FINAL_ROUND_EXIT = "AllRoundsPlayed"
ERRORS = re.compile(
    r"^(?:ERROR:|Fatal:|Sys_Error:|DOUBLE SIGNAL FAULT|----- Server Shutdown|SV_MapRestart_f.*dropped client)"
    r"|Server command overflow|G_Alloc: failed on allocation|Unable to create bot"
)
PLAYER_STATE = re.compile(r"^\d+: playerstate (.*)")
FULL_SNAPSHOT = re.compile(r"^snapshot:\d+\s+delta:-1\b")


def info_fields(info):
    fields = info.removeprefix("\\").split("\\")
    return dict(zip(fields[::2], fields[1::2]))


def read_line(log):
    offset = log.tell()
    raw = log.readline()
    if not raw.endswith(b"\n"):
        log.seek(offset)
        return None
    return raw.decode(errors="replace").strip().removeprefix("^1")


def send_command(process, command):
    process.stdin.write((command + "\n").encode())
    process.stdin.flush()


class Spectator:
    def __init__(self, address, process, log):
        self.address = address
        self.process = process
        self.log = log
        self.slot = None
        self.active = False
        self.followed = {}
        self.generation = 0
        self.fields = {}
        self.sampling = False
        self.next_sample = 0
        self.follow_deadline = 0

    def connect(self):
        self.follow_deadline = time.monotonic() + 35
        send_command(self.process, f"connect {self.address}")

    def reset(self, generation):
        self.active = False
        self.generation = generation
        if self.sampling:
            send_command(self.process, "set cl_shownet 0; set cl_nodelta 0")
        self.sampling = False
        self.fields = {}
        self.next_sample = 0
        self.follow_deadline = time.monotonic() + 35

    def update(self, bots, may_follow):
        now = time.monotonic()
        if self.process.poll() is not None:
            raise RuntimeError(f"Spectator client exited with status {self.process.returncode}")

        while (line := read_line(self.log)) is not None:
            if ERRORS.search(line) or line.startswith(("----- Client Shutdown", "Server disconnected")):
                raise RuntimeError(f"Spectator client: {line}")
            if not self.sampling:
                continue
            if state := PLAYER_STATE.match(line):
                self.fields = dict(re.findall(r"(\w+):(-?\d+)", state[1]))
            elif FULL_SNAPSHOT.match(line) and self.fields:
                # Zero fields are omitted in the client debug output for a full snapshot.
                viewed = int(self.fields.get("clientNum", 0))
                flags = int(self.fields.get("pm_flags", 0))
                following = flags & PMF_FOLLOW and viewed in bots
                stop_sample = "set cl_shownet 0; set cl_nodelta 0"
                if self.active and may_follow and following:
                    if self.generation not in self.followed:
                        print(f"  Spectator following bot slot {viewed} in initialization {self.generation}", flush=True)
                    self.followed[self.generation] = viewed
                    self.follow_deadline = now + 25
                elif self.active and may_follow:
                    stop_sample += "; cmd follownext"
                send_command(self.process, stop_sample)
                self.sampling = False
                self.fields = {}
                self.next_sample = now + (2 if following else 1.5)

        if not may_follow:
            return
        if now >= self.follow_deadline:
            raise RuntimeError(f"Spectator did not confirm a bot's view in initialization {self.generation}")
        if self.active and bots and not self.sampling and now >= self.next_sample:
            # Request one full snapshot to verify the follow flag and viewed client on the real client.
            send_command(self.process, "set cl_nodelta 1; set cl_shownet 3")
            self.sampling = True


def shipped_maps():
    """Read the map/mode declarations shipped with the game."""
    assets = ROOT / "wop"
    maps = {}
    for name in ARENAS:
        source = re.sub(r"(?m)^\s*//.*$", "", (assets / name).read_text(encoding="utf-8"))
        for entry in re.finditer(r"\{([^{}]*)\}", source):
            fields = dict(re.findall(r'([a-zA-Z]+)\s+"([^"]*)"', entry[1]))
            map_name = fields.get("map", "")
            modes = fields.get("type", "").split()
            if (not re.fullmatch(r"[a-zA-Z0-9_-]+", map_name) or map_name.casefold() in maps
                    or not modes or set(modes) - MODES.keys() or len(modes) != len(set(modes))):
                raise ValueError(f"Invalid or duplicate arena entry in {name}: {entry[0]}")
            maps[map_name.casefold()] = (map_name, modes)
    for extension in ("bsp", "aas"):
        shipped = {file.stem.casefold() for pack in MAP_PACKS for file in (assets / pack / "maps").glob(f"*.{extension}")}
        if maps.keys() != shipped:
            raise ValueError(f"Arena and {extension.upper()} lists differ: {', '.join(sorted(maps.keys() ^ shipped))}")
    if not maps or {mode for _, modes in maps.values() for mode in modes} != MODES.keys():
        raise ValueError("Official arena files must cover all multiplayer modes")
    return [maps[name] for name in sorted(maps)]


def match_limit(mode, ending, time_limit, point_limit):
    if ending == "time":
        return f"set timelimit {time_limit}"
    if mode not in ("ft", "lps"):
        return f"set pointlimit {point_limit}"
    return ""


def make_config(map_name, mode, ending, time_limit, point_limit, with_client):
    timed = ending == "time"
    config = [
        f"set sv_maxclients {NUM_BOTS + int(with_client)}",
        "set sv_fps 20",
        f"set g_gametype {MODES[mode]}",
        "set bot_enable 1",
        "set bot_minplayers 0",
        "set g_teamAutoJoin 0",
        "set g_doWarmup 1",
        "set g_warmup 20",
        "set g_warmupReady 0",
        "set g_suddenDeath 0",
        'set g_log "games.log"',
        "set g_logSync 1",
        "set timelimit 0",
        "set pointlimit 0",
        "set g_LPS_flags 0",
        f"set g_LPS_startlives {100 if timed else 2}",
        f"set g_ft_numRounds {3 if timed else 2}",
        "set g_ft_playAllRounds 1",
        f"map {map_name}",
        f'set nextmap "set pointlimit 0; set timelimit 0; map {map_name}"',
    ]
    # Stagger joins so each bot can drain server commands between joins.
    for i in range(NUM_BOTS):
        team = ("red", "blue")[i % 2] if MODES[mode] >= MODES["team"] else "free"
        config += [f"addbot {BOT_MODELS[i % len(BOT_MODELS)]} 3 {team} 0 MatrixBot{i + 1:02}", "wait 5"]
    config.append("echo WOP_MATRIX_BOTS_ADDED")
    limit = match_limit(mode, ending, time_limit, point_limit)
    if limit and not with_client:
        config.append(limit)
    return "\n".join(config) + "\n"


def watch_match(server, log, map_name, mode, ending, args, spectator):
    expected = TIME_EXIT if ending == "time" else (FINAL_ROUND_EXIT if mode == "ft" else POINT_EXIT)
    match_timeout = args.time_limit * 60 + 20 if ending == "time" else args.score_timeout
    pending_limit = match_limit(mode, ending, args.time_limit, args.point_limit) if spectator else ""
    phase = "warmup and bot start"
    deadline = time.monotonic() + 90
    generation = loads = actions = 0
    restart_from = loads_at_exit = 0
    last_exit = ""
    bots = set()
    seeded = aas_ready = False
    verify_until = None
    exits = []
    bot_errors = {}
    while True:
        now = time.monotonic()
        if now >= deadline:
            raise RuntimeError(f"Timeout during {phase} ({len(bots)}/{NUM_BOTS} bots, exits: {exits})")
        line = read_line(log)
        if line is None:
            if server.poll() is not None:
                raise RuntimeError(f"Server exited with status {server.returncode} during {phase}")
            if spectator:
                spectator.update(bots, phase in ("play", "bot check after transition"))
                if pending_limit and phase == "play" and generation in spectator.followed:
                    # Let the spectator reach a bot before a short match can end.
                    send_command(server, pending_limit)
                    pending_limit = ""
            if (verify_until is not None and now >= verify_until
                    and (not spectator or generation in spectator.followed)):
                result = {
                    "exits": exits, "bots": len(bots), "gameplay_events": actions,
                    "initializations": generation, "bot_errors": bot_errors,
                }
                if spectator:
                    result.update(spectator_slot=spectator.slot, followed_bots=spectator.followed)
                return result
            time.sleep(0.1)
            continue
        if ERRORS.search(line) or line.startswith("ClientDisconnect: "):
            raise RuntimeError(f"{phase} (exits: {exits}): {line}")
        if line.startswith("Error: "):
            bot_errors[line] = bot_errors.get(line, 0) + 1
            if bot_errors[line] == 1:
                print(f"  {line}", flush=True)
        elif line == "AAS initialized.":
            aas_ready = True
        elif line.startswith("Server: "):
            loads += 1
        elif line.startswith("InitGame: "):
            settings = info_fields(line.removeprefix("InitGame: "))
            if (settings.get("mapname", "").casefold() != map_name.casefold()
                    or settings.get("g_gametype") != str(MODES[mode])):
                raise RuntimeError(f"Unexpected map or mode in: {line}")
            generation += 1
            bots.clear()
            if spectator:
                spectator.reset(generation)
        elif spectator and line.startswith("ClientUserinfoChanged: "):
            _, slot, info = line.split(" ", 2)
            fields = info_fields(info)
            if fields.get("n") == SPECTATOR:
                if fields.get("t") != TEAM_SPECTATOR:
                    raise RuntimeError(f"Spectator joined a playing team: {line}")
                spectator.slot = int(slot)
        elif line.startswith("ClientBegin: "):
            slot = int(line.split()[1])
            if spectator and slot == spectator.slot:
                spectator.active = True
            else:
                bots.add(slot)
        elif line == "WOP_MATRIX_BOTS_ADDED":
            if len(bots) != NUM_BOTS or not aas_ready:
                raise RuntimeError(f"Bot setup incomplete ({len(bots)}/{NUM_BOTS} bots, AAS ready: {aas_ready})")
            seeded = True
        elif line.startswith("Exit: "):
            last_exit = line.removeprefix("Exit: ")
            if phase != "play" or len(bots) != NUM_BOTS or not actions:
                raise RuntimeError(f"Match ended before gameplay with all {NUM_BOTS} bots: {line}")
            if spectator and generation not in spectator.followed:
                raise RuntimeError(f"Round ended before the client confirmed bot follow: {line}")
            exits.append(last_exit)
            if ending == "time" and mode == "ft":
                if last_exit not in (ROUND_EXIT, TIME_EXIT):
                    raise RuntimeError(f"{exits}: Freeze Tag finished before reaching the timelimit")
            else:
                wanted = [ROUND_EXIT, FINAL_ROUND_EXIT] if expected == FINAL_ROUND_EXIT else [expected]
                if exits != wanted[:len(exits)]:
                    raise RuntimeError(f"Unexpected match ending: {exits}; expected {wanted}")
            print(f"  {last_exit}", flush=True)
            restart_from, loads_at_exit = generation, loads
            phase = "intermission and automatic transition"
            deadline = time.monotonic() + (60 if spectator else 40)
        elif phase == "play" and line.startswith(("Kill: ", "AddScore: ", "Item: ")):
            actions += 1

        if seeded and phase == "warmup and bot start" and generation >= 2 and len(bots) == NUM_BOTS:
            phase = "play"
            deadline = time.monotonic() + match_timeout
            print(f"  {NUM_BOTS} bots active after warmup", flush=True)
            if spectator:
                # shortcut: join after warmup until its bulk bot restart stops dropping network clients.
                spectator.connect()
        elif phase == "intermission and automatic transition" and generation > restart_from and len(bots) == NUM_BOTS:
            if last_exit == ROUND_EXIT:
                if loads != loads_at_exit:
                    raise RuntimeError("Freeze Tag did not restart the map between rounds")
                phase = "play"
                last_exit = ""
                deadline = time.monotonic() + match_timeout
            elif last_exit == expected:
                expect_reload = not (mode == "ft" and ending == "time")
                if loads != loads_at_exit + int(expect_reload):
                    raise RuntimeError("Unexpected automatic transition after match end")
                phase = "bot check after transition"
                verify_until = time.monotonic() + 2


def stop_process(process):
    if process.poll() is None:
        process.terminate()
        try:
            process.wait(5)
        except subprocess.TimeoutExpired:
            process.kill()
            process.wait()


def run_match(server_path, client_path, case_dir, map_name, mode, ending, args):
    home = case_dir / "home"
    game = home / "wop"
    game.mkdir(parents=True)
    scratch = case_dir / "scratch"
    scratch.mkdir()
    (game / "match-test.cfg").write_text(
        make_config(map_name, mode, ending, args.time_limit, args.point_limit, client_path is not None),
        encoding="utf-8",
    )
    base = ["+set", "fs_basepath", str(ROOT)]
    net = ["+set", "net_enabled", "0"]
    if client_path:
        with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as probe:
            probe.bind(("127.0.0.1", 0))
            port = str(probe.getsockname()[1])
        net = ["+set", "net_enabled", "1", "+set", "net_ip", "127.0.0.1"]
    server_command = [
        str(server_path), *base, *net, "+set", "fs_homepath", str(home),
        "+set", "dedicated", "1", "+set", "sv_pure", "0",
        "+set", "vm_game", "2", "+exec", "match-test.cfg",
    ]
    with ExitStack() as stack:
        def start(command, filename, stdin):
            path = case_dir / filename
            output = stack.enter_context(path.open("wb"))
            log = stack.enter_context(path.open("rb"))
            process = stack.enter_context(subprocess.Popen(
                command, cwd=case_dir, env={**os.environ, "TMPDIR": str(scratch)}, stdin=stdin,
                stdout=output, stderr=subprocess.STDOUT,
            ))
            stack.callback(stop_process, process)
            return process, log

        spectator = None
        if client_path:
            server_command += ["+set", "net_port", port]
        server, log = start(server_command, "server.log", subprocess.PIPE if client_path else subprocess.DEVNULL)
        if client_path:
            client_home = case_dir / "client-home"
            client_home.mkdir()
            client_command = [
                str(client_path), *base, *net, "+set", "fs_homepath", str(client_home),
                "+set", "net_port", "0", "+set", "vm_ui", "2", "+set", "vm_cgame", "2",
                "+set", "cl_renderer", "opengl2", "+set", "r_fullscreen", "0", "+set", "r_mode", "6",
                "+set", "com_playIntro", "0", "+set", "name", SPECTATOR, "+setu", "team", "spectator",
            ]
            spectator = Spectator(f"127.0.0.1:{port}", *start(client_command, "client.log", subprocess.PIPE))
        return watch_match(server, log, map_name, mode, ending, args, spectator)


def positive_int(value):
    number = int(value)
    if number < 1:
        raise argparse.ArgumentTypeError("expected a positive integer")
    return number


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--server", type=Path, help="dedicated server executable (default: wopded.* in the checkout)")
    parser.add_argument("--with-client", action="store_true", help="spectate bots with a graphical OpenGL2 client")
    parser.add_argument("--client", type=Path, help="spectator client executable (implies --with-client, default: wop.*)")
    parser.add_argument("--output", type=Path, default=ROOT / "build/wop-match-test", help="parent directory for run logs")
    parser.add_argument("--map", action="append", help="limit to this map (repeatable)")
    parser.add_argument("--mode", action="append", choices=MODES, help="limit to this arena mode (repeatable)")
    parser.add_argument("--ending", choices=("time", "score"), help="default: test both endings")
    parser.add_argument("--time-limit", type=positive_int, default=1, help="timelimit in minutes (default: 1)")
    parser.add_argument("--point-limit", type=positive_int, default=1, help="pointlimit for scoring modes (default: 1)")
    parser.add_argument("--score-timeout", type=positive_int, default=300, help="seconds per scoring round (default: 300)")
    parser.add_argument("--list", action="store_true", help="print the test plan")
    args = parser.parse_args()
    if args.time_limit > (2**31 - 1) // 60000 or args.point_limit > 2**31 - 1:
        parser.error("game limits must fit the server's integer range")

    try:
        maps = shipped_maps()
    except (OSError, ValueError) as error:
        parser.error(str(error))
    selected = {name.casefold() for name in args.map or ()}
    unknown = selected - {name.casefold() for name, _ in maps}
    if unknown:
        parser.error(f"unknown official map: {', '.join(sorted(unknown))}")
    cases = [
        (name, mode, ending) for name, modes in maps if not selected or name.casefold() in selected
        for mode in modes if not args.mode or mode in args.mode
        for ending in ((args.ending,) if args.ending else ("time", "score"))
    ]
    if not cases:
        parser.error("no official map/mode combinations match the filters")
    with_client = args.with_client or args.client is not None
    extra = " and one spectator client" if with_client else ""
    print(f"{len(cases)} cases on {len({case[0] for case in cases})} maps, {NUM_BOTS} bots{extra} each", flush=True)
    if args.list:
        for name, mode, ending in cases:
            print(f"{name:24} {mode:8} {ending}")
        return 0

    binaries = sorted(path for path in ROOT.glob("wopded.*") if path.is_file() and os.access(path, os.X_OK))
    server = args.server or (binaries[0] if len(binaries) == 1 else None)
    if server is None or not server.is_file() or not os.access(server, os.X_OK):
        parser.error("build the dedicated server and qagame QVM, or provide --server")
    if not (ROOT / "wop/vm/qagame.qvm").is_file():
        parser.error("build the qagame QVM (CMake target qvm_qagame)")
    server = server.resolve()
    client = None
    if with_client:
        if os.name == "nt":
            parser.error("--with-client needs a WoP build that reads console commands from stdin")
        binaries = sorted(path for path in ROOT.glob("wop.*") if path.is_file() and os.access(path, os.X_OK))
        client = args.client or (binaries[0] if len(binaries) == 1 else None)
        if client is None or not client.is_file() or not os.access(client, os.X_OK):
            parser.error("build the graphical client or provide --client")
        if not all((ROOT / f"wop/vm/{module}.qvm").is_file() for module in ("ui", "cgame")):
            parser.error("build the UI and cgame QVMs (CMake targets qvm_ui and qvm_cgame)")
        client = client.resolve()
    args.output.mkdir(parents=True, exist_ok=True)
    run = Path(tempfile.mkdtemp(prefix="run-", dir=args.output.resolve()))
    print(f"Log directory: {run}", flush=True)
    failures = 0
    with (run / "results.jsonl").open("w", encoding="utf-8", buffering=1) as results:
        for index, (name, mode, ending) in enumerate(cases, 1):
            case = f"{index:03}-{name}-{mode}-{ending}"
            print(f"[{index}/{len(cases)}] {name} {mode} {ending}", flush=True)
            start = time.monotonic()
            entry = {"map": name, "mode": mode, "ending": ending, "log": f"{case}/server.log"}
            if client:
                entry["client_log"] = f"{case}/client.log"
            try:
                entry.update(run_match(server, client, run / case, name, mode, ending, args))
                if entry["bot_errors"]:
                    entry.update(status="failed", error="Match finished with bot library errors")
                    failures += 1
                else:
                    entry["status"] = "passed"
            except (OSError, RuntimeError, ValueError) as error:
                entry.update(status="failed", error=str(error))
                failures += 1
                print(f"  FAIL: {error}", flush=True)
            entry["seconds"] = round(time.monotonic() - start, 2)
            results.write(json.dumps(entry) + "\n")
            os.fsync(results.fileno())
    print(f"{len(cases) - failures} passed, {failures} failed", flush=True)
    return int(failures > 0)


if __name__ == "__main__":
    raise SystemExit(main())
