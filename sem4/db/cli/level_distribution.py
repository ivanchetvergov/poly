from __future__ import annotations


LEVEL_COEFFICIENTS: dict[str, dict[str, float]] = {
    "level1": {
        "users": 0.644,
        "datasets": 0.096,
        "dataset_files": 0.08,
        "competitions": 0.18,
    },
    "level2": {
        "configurations": 0.08,
        "competition_datasets": 0.07,
        "teams": 0.18,
        "team_members": 0.37,
        "team_competitions": 0.05,
        "participations": 0.25,
    },
    "level3": {
        "submissions": 1.0,
    },
}


ALL_LEVEL_RATIOS: dict[str, float] = {
    "level1": 0.43,
    "level2": 0.39,
    "level3": 0.18,
}


def distribute_total(total_count: int, ratios: dict[str, float]) -> dict[str, int]:
    if total_count <= 0:
        raise ValueError("total_count must be greater than 0")
    if not ratios:
        raise ValueError("ratios must not be empty")

    ratio_sum = sum(ratios.values())
    if ratio_sum <= 0:
        raise ValueError("sum of ratios must be greater than 0")

    normalized = {key: value / ratio_sum for key, value in ratios.items()}

    base: dict[str, int] = {}
    fractions: list[tuple[float, str]] = []
    allocated = 0

    for key, ratio in normalized.items():
        raw = total_count * ratio
        integer = int(raw)
        base[key] = integer
        allocated += integer
        fractions.append((raw - integer, key))

    remainder = total_count - allocated
    for _fraction, key in sorted(fractions, reverse=True)[:remainder]:
        base[key] += 1

    return base


def level_counts(level: str, total_count: int) -> dict[str, int]:
    if level not in LEVEL_COEFFICIENTS:
        raise ValueError(f"Unknown level: {level}")
    return distribute_total(total_count, LEVEL_COEFFICIENTS[level])
