from config import DATA_DIR
import random
from pathlib import Path

def validate_weekday(weekday):
    valid = {"Mon","Tue","Wed","Thu","Fri","Sat","Sun"}

    for day in weekday[:]: # iterate over a copy
        if day not in valid:
            weekday.remove(day)

def validate_period(period):
    valid = {1, 2, 3, 4, 5, 6}

    for p in period[:]:  
        if p not in valid:
            period.remove(p)

def query_range(name):
    print(f"\n{name} sweep")

    start = float(input("  Start: "))
    stop = float(input("  Stop: "))
    step = float(input("  Step: "))

    values = []
    if start == stop:
        values.append(round(start, 8))
        return values

    x = start
    while x <= stop:
        values.append(round(x, 8))
        x += step

    return values

# Unused for now
def query_seeds():
    choice = input(
        "\nRandom seeds or explicit list? [random/list]: "
    ).strip().lower()

    if choice == "list":
        return [int(x) for x in input("Seeds: ").split()]

    n = int(input("Number of random seeds: "))
    return [random.randint(0, 2**31 - 1) for _ in range(n)]

def query_seed():
    while True:
        try:
            return int(input("\nRandom seed: "))
        except ValueError:
            print("Please enter an integer.")

def query_input_file() -> str:
    while True:
        filename = input("Input filename: ").strip()
        path = Path(DATA_DIR) / filename

        if not path.exists():
            print("File does not exist.")
            continue

        return str(path)

def query_output_file() -> str:
    while True:
        filename = input("Output filename: ").strip()
        path = Path(DATA_DIR) / filename

        if path.exists():
            print("File already exists.")
            continue

        return str(path)
