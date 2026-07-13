import pandas as pd
import matplotlib.pyplot as plt
from queries import (
    query_input_file,
    query_output_file
)

def plot_1d(df: pd.DataFrame, parameter: str, metric="finalScore", output_file=None):
    summary = (
        df.groupby(parameter)[metric]
          .mean()
          .reset_index()
          .sort_values(parameter)
    )

    plt.figure(figsize=(8, 5))
    plt.plot(summary[parameter], summary["finalScore"], marker="o")

    plt.xlabel(parameter)
    plt.ylabel(f"{metric}")
    plt.title(f"{metric} vs {parameter}")
    plt.grid(True)

    
    plt.tight_layout()

    if output_file is not None:
        plt.savefig(output_file, dpi=300, bbox_inches="tight")
    plt.show()

    plt.close()


def plot_heatmap(df: pd.DataFrame, x: str, y: str, metric="finalScore", output_file=None):
    pivot = df.pivot_table(
        values=metric,
        index=y,
        columns=x,
        aggfunc="mean"
    )

    plt.figure(figsize=(8, 6))

    plt.imshow(
        pivot.values,
        origin="lower",
        aspect="auto"
    )

    plt.colorbar(label=f"Mean {metric}")

    plt.xticks(
        range(len(pivot.columns)),
        pivot.columns,
        rotation=45
    )

    plt.yticks(
        range(len(pivot.index)),
        pivot.index
    )

    plt.xlabel(x)
    plt.ylabel(y)
    plt.title(f"Final Score ({y} vs {x})")

    plt.tight_layout()

    if output_file is not None:
        plt.savefig(output_file, dpi=300, bbox_inches="tight")

    plt.show()
    plt.close()

def query_graph():
    while True:
        mode = input("\nGraph ([u]nivariate/[h]eatmap): ").strip().lower()

        if mode in ("u", "univariate"):
            return "univariate"

        if mode in ("h", "heatmap"):
            return "heatmap"

        print("Invalid mode.")

def graph():
    graph_type = query_graph()

    input_file = query_input_file()
    output_file = query_output_file()

    df = pd.read_csv(input_file)

    if graph_type == "univariate":
        parameter = input(
            "\nHyperparameter (init_temp_siman, cooldown_siman, k_max): "
        ).strip()

        plot_1d(
            df=df,
            parameter=parameter,
            output_file=output_file,
        )

    elif graph_type == "heatmap":
        x = input("\nX hyperparameter: ").strip()
        y = input("Y hyperparameter: ").strip()

        plot_heatmap(
            df=df,
            x=x,
            y=y,
            output_file=output_file,
        )
