---
title: "Algorithms and Data Structure"
sub_title: "Final Presentation"
authors: 
    - Hoang Huu Khiem ~ 1W24CF17
    - Yukihiro Mae ~ 1W24CF19
    - of East Hokusai Group
theme: 
    name: catppuccin-mocha
---

<!-- // Slide 2 -->
The Problem (definition)
===
<!-- column_layout: [2, 1, 2] -->
<!-- column: 0 -->
<!-- font_size: 2 -->
It's well-known that combinatorial problems are hard
<!-- pause -->
Let's do a simple illustration
<!-- pause -->

<!-- font_size: 1 -->

```python {1 - 7 | 6 | all} +line_numbers +exec
import time
from itertools import permutations

for n in range(6, 13):
    start = time.time()
    list(permutations(range(n)))
    print(n, time.time() - start)
```

<!-- pause -->
<!-- font_size: 2 -->
=> Conclusion: not ideal

<!-- pause -->

<!-- column: 2 -->
There are a lot of problems like these.

<!-- pause -->

This includes problems like scheduling our time
so we make the best of it. 

<!-- pause -->

How do we waste the least of our time 
doing the most valuable activities?

<!-- pause -->

<!-- reset_layout -->
In essence, this is the problem we are tackling today. 
To state it more formally:

<span style="color: #ffee8c">Problem</span>: creating a course registration program, 
where a schedule plan is created for a
semester that maximally satisfies a student's
preferences.

<!-- pause -->

<span style="color: #7FFFD4">Implementation</span>: CLI that reads courses from a file
and try to
<!-- incremental_lists: true -->
* Meet the student's target credit count
* Restrict certain days or periods in the schedule according to the student's preferences
* Create a reasonable schedule that neither burns the student out or make them waste their time

<!-- end_slide -->

<!-- // Slide 3 -->
How to approach?
===
<!-- font_size: 2 -->
Because of the limitations of combinatorial problems mostly being NP-hard or NP-complete, it's not
feasible for us to use any sort of brute-force algorithm to search the entire solution space.

<!-- alignment: center -->
=> Some alternatives: pick local best (greedy) or a stochastic search
<!-- reset_layout -->

<!-- pause -->

<!-- column_layout: [4, 1, 4] -->
<!-- column: 0 -->
# Greedy
* Simple implementation
* "If locally optimal then take without backtracking"
* This local subproblem is usually very easy and fast

<!-- pause -->

## Our implementation: 
* Sort the course list in descending order by some criterion (we chose credit count)
* Take top courses in the sorted list; skip any course with schedule conflicts
* Do this until we reach the required credit count
<!-- pause -->

<!-- column: 2 -->
<!-- font_size: 2 -->
# Stochastic search
* Stochastic, so based on randomness
* These usually have metaheuristics, or hyperparameters, that the programmer has to
tune

<!-- pause -->

## Our implementation:
* A simulated annealing (SimAn) algorithm, inspired by metallurgy
* We supply an initial temperature and cooldown parameter to
simulate the process of metal cooling
* Through this process metal reaches a low energy state that is 
very stable -> maybe we achieve the same thing with schedules

<!-- end_slide -->

<!-- // Slide 4 -->
Algorithm description: More about greedy
===
<!-- font_size: 2 -->
# Greedy internals
Greedy fits in the entire function below:

<!-- new_lines: 3 -->

<!-- font_size: 1 -->
```c {7 | 10 - 16 | 17 - 20 | all} +line_numbers
// O(nlogn) where n is number of courses
Schedule* maximizeCreditsDumb(CourseList* courseList, MasterCheck* mastercheck) {
    Schedule* schedule = createSchedule();
    if (schedule == NULL) return NULL;

    // sort course list by credits highest -> lowest
    sortCourseList(courseList, "credits");

    size_t increment = 0;
    while((schedule->totalCredits) < (mastercheck->targetCredits) && increment < courseList->courseCountTotal){
        bool check = addCourseToSchedule(schedule, courseList->courseList[increment]);
        if (check == false) {
            increment++;
            continue;
        }
    }
    double cost = objective(schedule, courseList, mastercheck);
    printf("Greedy (credits) algorithm arrived at schedule with cost: %lf\n", cost);
    return schedule;
}
```
<!-- font_size: 2 -->
<!-- new_lines: 3 -->
Time complexity of sorting is O(nlogn), so that's the time complexity of the entire algorithm.
<!-- end_slide -->

<!-- // Slide 5 -->
Algorithm description: More about greedy
===
<!-- font_size: 1 -->
# Greedy externals
Let's test it out on a random dataset:

<!-- font_size: 1 -->
```bash +exec
EXEC=/home/gump/Documents/school/sem4/dsa/presentation/build/ehs
time $EXEC -r -a greedy -t 21 --weekday-bad=Sat --period-bad=1,5,6 --output="greedy.csv"
```

<!-- end_slide -->

<!-- // Slide 6 -->
Cost & the Objective Function
===
<!-- font_size: 2 -->
To compare different algorithms to each other, in the sense of how "good" they are,
we create a function called the objective function.

The objective function is defined through a set of hard and soft requirements about
each schedule. A lower objective function is considered better. In general, objective 
functions differ based on the problem description.

<!-- font_size: 1 -->

```c
/* Scores a specific schedule based on a few criteria (lower is better):*
* - Starts from 0
* - If no courses or credits are taken at all: immediate worst score
* - For each gap between two courses on the same day: somewhat bad penalty for each gap, increasing linearly
* - For each day going to school: quite bad penalty for each day, increasing linearly 
* - Under/over credit: plus/minus 2 away from desired credit is low penalty, higher penalty as you move away
* - Class in a user-designated bad period: very bad penalty, scales o(superlinearly) 
*                                          with amount of classes in bad periods 
* - Not taking all required credits: quadratic penalty if lacking, no penalty else. estimates the number of 
*                                    required courses aimed for in a semester over 4 years is total number of 
*                                    required courses / 8. This difference is squared and then multiplied with BAD_PENALTY
* - 1 slot taken in a day or >3 taken in a day: only 1 slot taken: BAD_PENALTY
*                                               >3 slot taken: exponential penalty for each course over the 3rd, I *know* how terrible this is */
double objective(Schedule* schedule, CourseList* courseList, MasterCheck* mastercheck) {
    double score = 0;
    if (schedule->courseCountTaken == 0 || schedule->totalCredits == 0)
         score += ASTRONOMICAL_PENALTY;

    score += gapPenalty(schedule);
    score += commutePenalty(schedule);
    score += creditDivergencePenalty(schedule, mastercheck);
    score += badSlotPenalty(schedule, mastercheck);
    score += requirementPenalty(schedule, courseList);
    score += courseCountPenalty(schedule);

    return score;
}
```
<!-- font_size: 2 -->
Because a good objective function captures the high dimensional input of the problem
space in a single output, designing a good objective function is 
<span style="color: #850101">***critically important***</span> for the design of any
metaheuristic algorithm.

<!-- end_slide -->

<!-- // Slide 7 -->
Simulated Annealing (SimAn)
===
<!-- font_size: 2 -->
# SimAn internals
SimAn directly utilises the aforementioned cost function in operation.

The code we wrote is too long, but the pseudocode can be followed as:

<!-- font_size: 1 -->
```c {7 | 10 - 31 | 12 | 15-16 | 19 | 22 - 28 | 30 | all} +line_numbers
Schedule simulatedAnnealing(double INIT_TEMP, double COOLDOWN, int k_max) {
    double temperature = INIT_TEMP;
    double p_accept;
    double current_cost, new_cost;

    // build a random schedule
    Schedule current = buildRandomSchedule(courseList);

    // loop until temperature is cooled down totally
    for (int i = 0; i < k_max; i++) {
        // copy schedule into neighbor
        Schedule neighbor = makeNeighbor(current);

        // calculate cost of current solution and cost of new solution to compare
        current_cost = objective(current);
        new_cost = objective(neighbor);

        // a probability to help not looping forever
        p_accept = acceptanceProbability(current_cost, new_cost, temperature);

        // check if the probability function is larger than some probability between 0 and 1
        if (p_accept > probUniform(0, 1)) {
            // if so then update neighbor to be the new solution
            current = neighbor;
            current_cost = new_cost;
        }
        else
            destroy(neighbor);
        // cool down
        temperature *= COOLDOWN;
    }
    return current;
}
```

<!-- font_size: 2 -->
The idea is simple, but even some functions are tunable parameters. Does it hold up?

<!-- end_slide -->

<!-- // Slide 8 -->
Simulated Annealing (SimAn)
===
<!-- font_size: 2 -->
Let's run the code one more time:
<!-- font_size: 1 -->
```bash +exec
EXEC=/home/gump/Documents/school/sem4/dsa/presentation/build/ehs
time $EXEC -r -a siman -t 21 --weekday-bad=Sat --period-bad=1,5,6 --cooldown=0.995 --temp=500 --k-max=100 --output="siman.csv"
```

<!-- end_slide -->

<!-- // Slide 9 -->
Findings
===
<!-- font_size: 2 -->
Tuning the parameters with a Python script and feeding them into the executable made with C as commandline args, 
we ran a few thousand trials on different settings of hyperparameters to see how they affect the annealment.

Here's it for sweeping over init_temp:

<!-- font_size: 1 -->
```python +exec 
import pandas as pd
def plot_1d_ascii(df: pd.DataFrame, parameter: str, metric="finalScore", width=60, height=15):
    summary = df.groupby(parameter)[metric].mean().reset_index().sort_values(parameter)
    xs = summary[parameter].tolist()
    ys = summary[metric].tolist()
    x_min, x_max = min(xs), max(xs)
    y_min, y_max = min(ys), max(ys)
    grid = [[' '] * width for _ in range(height)]
    for x, y in zip(xs, ys):
        col = int((x - x_min) / (x_max - x_min + 1e-9) * (width - 1))
        row = int((1 - (y - y_min) / (y_max - y_min + 1e-9)) * (height - 1))
        grid[row][col] = '*'
    print(f"{metric} vs {parameter}")
    print(f"{y_max:.2f} |" + " " * width)
    for row in grid:
        print("      |" + "".join(row))
    print("      +" + "-" * width)
    print(f"{y_min:.2f}" + " " * (width - 5) + f" {x_min:.2f}{' ' * (width-15)}{x_max:.2f}")
df = pd.read_csv("/home/gump/Documents/school/sem4/dsa/presentation/slides/sweep_temp.csv")
plot_1d_ascii(df, parameter="init_temp_siman", metric="finalScore")
```

<!-- end_slide -->

<!-- // Slide 10 -->
Findings
===
<!-- font_size: 2 -->
Here's a non-vanity version of it:
![](../assets/sweep_temp.png)

<!-- end_slide -->

<!-- // Slide 11 -->
Findings
===
<!-- font_size: 2 -->
That's quite chaotic! This is the graph for cooldown:

<!-- font_size: 1 -->
```python +exec 
import pandas as pd
def plot_1d_ascii(df: pd.DataFrame, parameter: str, metric="finalScore", width=60, height=15):
    summary = df.groupby(parameter)[metric].mean().reset_index().sort_values(parameter)
    xs = summary[parameter].tolist()
    ys = summary[metric].tolist()
    x_min, x_max = min(xs), max(xs)
    y_min, y_max = min(ys), max(ys)
    grid = [[' '] * width for _ in range(height)]
    for x, y in zip(xs, ys):
        col = int((x - x_min) / (x_max - x_min + 1e-9) * (width - 1))
        row = int((1 - (y - y_min) / (y_max - y_min + 1e-9)) * (height - 1))
        grid[row][col] = '*'
    print(f"{metric} vs {parameter}")
    print(f"{y_max:.2f} |" + " " * width)
    for row in grid:
        print("      |" + "".join(row))
    print("      +" + "-" * width)
    print(f"{y_min:.2f}" + " " * (width - 5) + f" {x_min:.2f}{' ' * (width-15)}{x_max:.2f}")
df = pd.read_csv("/home/gump/Documents/school/sem4/dsa/presentation/slides/sweep_cd.csv")
plot_1d_ascii(df, parameter="cooldown_siman", metric="finalScore")
```
<!-- end_slide -->

<!-- // Slide 12 -->
Findings
===
<!-- font_size: 2 -->
Here's a non-vanity version of it:
![](../assets/sweep_cd.png)

<!-- end_slide -->

<!-- // Slide 13 -->
Findings
===
<!-- font_size: 2 -->
So maybe temperature is king? The lower the better it seemed, and cooldown did not seem to matter much
except near cooldown = 1 (when temperature would basically not change and result in practically a random
walk)

To confirm, we varied init_temp and cooldown at once (no fancy ASCII here)
![](../assets/sweep_cd_temp.png)

<!-- end_slide -->

<!-- // Slide 14 -->
Findings
===
<!-- font_size: 2 -->
So it seems like only at a given level of cooldown (near 1) does temperature fluctation matter

Are hyperparameters just kind of useless? So we varied k_max for completeness:
<!-- font_size: 1 -->
```python +exec 
import pandas as pd
def plot_1d_ascii(df: pd.DataFrame, parameter: str, metric="finalScore", width=60, height=15):
    summary = df.groupby(parameter)[metric].mean().reset_index().sort_values(parameter)
    xs = summary[parameter].tolist()
    ys = summary[metric].tolist()
    x_min, x_max = min(xs), max(xs)
    y_min, y_max = min(ys), max(ys)
    grid = [[' '] * width for _ in range(height)]
    for x, y in zip(xs, ys):
        col = int((x - x_min) / (x_max - x_min + 1e-9) * (width - 1))
        row = int((1 - (y - y_min) / (y_max - y_min + 1e-9)) * (height - 1))
        grid[row][col] = '*'
    print(f"{metric} vs {parameter}")
    print(f"{y_max:.2f} |" + " " * width)
    for row in grid:
        print("      |" + "".join(row))
    print("      +" + "-" * width)
    print(f"{y_min:.2f}" + " " * (width - 5) + f" {x_min:.2f}{' ' * (width-15)}{x_max:.2f}")
df = pd.read_csv("/home/gump/Documents/school/sem4/dsa/presentation/slides/sweep_kmax.csv")
plot_1d_ascii(df, parameter="k_max", metric="finalScore")
```

<!-- end_slide -->

<!-- // Slide 15 -->
Findings
===
<!-- font_size: 2 -->
k-max matters, and it matters a ***ton***. SA converges fast with respect to k_max.
This is good news, as the time complexity is roughly O(k_max * n), where n is the number
of data points. 

SimAn reached good performance at around the hundreds (~the order of magnitude
of init_temp, which was set at 300.0)

![](../assets/sweep_kmax.png)

<!-- end_slide -->

<!-- // Slide 16 -->
Notable runs: Random Walk
===
```bash +exec
EXEC=/home/gump/Documents/school/sem4/dsa/presentation/build/ehs
time $EXEC -r -a siman -t 21 --weekday-bad=Sat --period-bad=1,5,6 --cooldown=1 --temp=100000 --k-max=100 --output="siman.csv"
```

<!-- end_slide -->

<!-- // Slide 16 -->
Notable runs: Frozen Metal
===
```bash +exec
EXEC=/home/gump/Documents/school/sem4/dsa/presentation/build/ehs
time $EXEC -r -a siman -t 21 --weekday-bad=Sat --period-bad=1,5,6 --cooldown=0 --temp=100000 --k-max=100 --output="siman.csv"
```

<!-- end_slide -->

<!-- // Slide 18 -->
Notable runs: k-maxxing
===
```bash +exec
EXEC=/home/gump/Documents/school/sem4/dsa/presentation/build/ehs
# score is shockingly low!
time $EXEC -r -a siman -t 21 --weekday-bad=Sat --period-bad=1,5,6 --cooldown=0.999 --temp=100000 --k-max=5000 --output="siman.csv"
```
<!-- end_slide -->

<!-- // Slide 17 -->
Notable runs: k-fail (Random walk 2)
===
```bash +exec
EXEC=/home/gump/Documents/school/sem4/dsa/presentation/build/ehs
# score is shockingly high! k_max is doubled, temp is the same, but cooldown ~ 1, and that was enough to knock it out
time $EXEC -r -a siman -t 21 --weekday-bad=Sat --period-bad=1,5,6 --cooldown=0.9999 --temp=100000 --k-max=10000 --output="siman.csv"
```

<!-- end_slide -->

The End
===
<!-- jump_to_middle -->
<!-- font_size: 3 -->
Thanks for listening!
---
