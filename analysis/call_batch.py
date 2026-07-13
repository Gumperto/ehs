from config import EHS_BINARY_PATH
import sys
import subprocess

from queries import (
    validate_period,
    validate_weekday,
    query_input_file,
    query_output_file,
    query_range,
    query_seed,
)

def call_one(path="./build/ehs", algorithm="siman", filename=None, output=None, weekday=None, period=None, target_credits=20, seed=None, hyperparams=None):
    if weekday is None:
        weekday = ["Mon", "Sat"]
    if period is None:
        period = ["1","5","6"]

    cmd = [path, f"--algorithm={algorithm}"]

    if filename is None:
        cmd.append("-r")
    else:
        cmd.append(f"--data={filename}")

    if seed is not None:
        cmd.append(f"--seed={seed}")

    if output is not None:
        cmd.append(f"--output={output}")
    
    if algorithm == "siman":
        if hyperparams is None:
            hyperparams = {
                "initTemp": 300,
                "cooldown": 0.995,
                "k_max"   : 1000
            }

        cmd += [
            f"--weekday-bad={','.join(weekday)}",
            f"--period-bad={','.join(str(p) for p in period)}",
            f"--target-credits={target_credits}",
            f"--cooldown={hyperparams["cooldown"]}",
            f"--temp={hyperparams["initTemp"]}",
            f"--k-max={hyperparams["k_max"]}",
        ]

        return subprocess.run(cmd, capture_output=True, text=True)

    else:
        print("No valid algorithm chosen")
        return 0

def sweep(path="./build/ehs", algorithm="siman", filename=None, output=None, weekday=None, period=None, target_credits=20, seed=None, hyperparams_list=None):
    total = 0
    error = 0
    cancel = 1
    if algorithm == "siman": 
        if hyperparams_list is None:
            hyperparams_list = {
                "cooldown_list": [0.995],
                "temp_list"    : [300.0],
                "k_max_list"   : [1000]
            }

        for cooldown in hyperparams_list["cooldown_list"]:
            for temp in hyperparams_list["temp_list"]:
                for k_max in hyperparams_list["k_max_list"]:
                    hyper = {
                        "initTemp": temp,
                        "cooldown": cooldown,
                        "k_max"   : k_max
                    }
                    check = call_one(
                                path=path, 
                                algorithm=algorithm, 
                                filename=filename, 
                                output=output, 
                                weekday=weekday, 
                                period=period, 
                                target_credits=target_credits, 
                                seed=seed, 
                                hyperparams=hyper
                            );
                    if check == 1:
                        print(f"WARNING! Error happened on run {total}\n")
                        if cancel == 1:
                            cancel_char = input("Cancel job [y/n]? This will not appear again: ")
                            if cancel_char == 'y':
                                sys.exit(1)
                            cancel = 0
                        error += 1
                        total += 1
                    else:
                        total += 1
        print(f"Sweep complete: {total - error}/{total} OK")

    else:
        print("No valid algorithm chosen")

# only siman is supported right now
def query_sweep():
    print("=== Sweep Configuration ===")
    input_file = query_input_file()
    output_file = query_output_file()
    print(f"Results will be saved to {output_file}")

    # Days
    weekday = input(
        "Days to discourage (e.g. Mon Tue Fri, leave blank for none): "
    ).split()
    validate_weekday(weekday)

    # Periods
    period = [int(x) for x in input(
        "Periods to discourage (e.g. 1 2 5, leave blank for none): "
    ).split()]
    validate_period(period)

    # Credits
    while True:
        try:
            target_credits = int(input("Target credits [20]: ") or 20)
            if target_credits > 0:
                break
        except ValueError:
            pass
        print("Please enter a positive integer.")

    # Hyperparameter ranges
    cooldown = query_range("Cooldown")
    temperature = query_range("Temperature")
    kmax = query_range("kmax")

    seed = query_seed()

    hyper = {
        "cooldown_list": cooldown, 
        "temp_list"    : temperature,
        "k_max_list"   : kmax
    }

    sweep(
        path=EHS_BINARY_PATH, 
        algorithm="siman", 
        filename=input_file, # this means it's random 
        output=output_file,
        weekday=weekday, 
        period=period, 
        target_credits=target_credits, 
        seed=seed, 
        hyperparams_list=hyper
    )
