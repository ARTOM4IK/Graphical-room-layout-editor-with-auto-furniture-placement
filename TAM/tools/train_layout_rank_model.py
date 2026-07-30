#!/usr/bin/env python3
"""
 ***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************

 FILE NAME   : train_layout_rank_model.py
 PURPOSE     : Tough Arrangement Manager project.
               Layout rank model training script.
 PROGRAMMER  : CGSG'Sr'2025.
               Arsentev Artemy (AA4).
 LAST UPDATE : 21.04.2026
 NOTE        : None.

 No part of this file may be changed without agreement of
 Computer Graphics Support Group of 30 Phys-Math Lyceum
"""

from __future__ import annotations

import argparse
import json
import math
import random
from pathlib import Path
from typing import Dict, List, Tuple


FEATURE_ORDER: List[str] = [
    "prefer_wall",
    "prefer_corner",
    "keep_center_free",
    "is_large",
    "role_anchor",
    "role_storage",
    "role_table",
    "role_seating",
    "room_bathroom",
    "room_dining_or_kitchen",
    "wall_closeness",
    "center_closeness",
    "door_clearance",
    "window_clearance",
    "corner_closeness",
    "wall_aligned",
    "faces_room_center",
    "has_table_context",
    "table_distance_fit",
    "table_facing_alignment",
]

BOOLEAN_FEATURES = {
    "prefer_wall",
    "prefer_corner",
    "keep_center_free",
    "is_large",
    "role_anchor",
    "role_storage",
    "role_table",
    "role_seating",
    "room_bathroom",
    "room_dining_or_kitchen",
    "wall_aligned",
    "has_table_context",
}

"""Clamp one value into [0; 1].
* ARGUMENTS:
*   - value:
*       float value;
* RETURNS:
*   (float) bounded value between 0.0 and 1.0.
"""
def clamp01(value: float) -> float:
    return max(0.0, min(1.0, value))
""" End of 'clamp01' function """

"""Create one feature dictionary with zero defaults.
* ARGUMENTS:
*   - kwargs:
*       float keyword arguments;
* RETURNS:
*   (dict) dictionary of features mapping strings to floats.
"""
def make_features(**kwargs: float) -> Dict[str, float]:
    features = {name: 0.0 for name in FEATURE_ORDER}
    for key, value in kwargs.items():
        if key in features:
            features[key] = float(value)
    return features
""" End of 'make_features' function """

"""Generate one nearby synthetic example around a scenario prototype.
* ARGUMENTS:
*   - base:
*       dict reference to baseline feature configuration;
*   - rng:
*       random.Random generation source instance;
* RETURNS:
*   (dict) adjusted floating-point feature parameters.
"""
def jitter_features(base: Dict[str, float], rng: random.Random) -> Dict[str, float]:
    result: Dict[str, float] = {}

    for feature_name in FEATURE_ORDER:
        value = float(base[feature_name])

        if feature_name in BOOLEAN_FEATURES:
            result[feature_name] = 1.0 if value >= 0.5 else 0.0
            continue

        noise = 0.045
        if feature_name in {"table_distance_fit", "table_facing_alignment", "faces_room_center"}:
            noise = 0.06
        if feature_name in {"door_clearance", "window_clearance"}:
            noise = 0.07

        result[feature_name] = clamp01(value + rng.uniform(-noise, noise))
    return result
""" End of 'jitter_features' function """

"""Build a deterministic synthetic expert-labeled dataset.
* ARGUMENTS:
*   - seed:
*       int pseudo-random generation initialization value;
* RETURNS:
*   (dict) final constructed collection of layout evaluation scenarios.
"""
def generate_dataset(seed: int) -> Dict[str, object]:
    rng = random.Random(seed)
    samples: List[Dict[str, object]] = []

    scenarios = [
        {
            "group_id": "chair_dining_01",
            "room_type": "dining_room",
            "asset_name": "chair",
            "good_count": 8,
            "bad_count": 8,
            "good_comment": "Chair stays close to dining table and clearly faces it.",
            "bad_comment": "Chair drifts away from the dining table and turns aside.",
            "good": make_features(
                role_seating=1, room_dining_or_kitchen=1, wall_closeness=0.36,
                center_closeness=0.48, door_clearance=0.84, window_clearance=0.78,
                corner_closeness=0.22, wall_aligned=1, faces_room_center=0.68,
                has_table_context=1, table_distance_fit=0.96, table_facing_alignment=0.97
            ),
            "bad": make_features(
                role_seating=1, room_dining_or_kitchen=1, wall_closeness=0.82,
                center_closeness=0.21, door_clearance=0.54, window_clearance=0.39,
                corner_closeness=0.61, wall_aligned=1, faces_room_center=0.18,
                has_table_context=1, table_distance_fit=0.15, table_facing_alignment=0.11
            ),
        },
        {
            "group_id": "chair_office_01",
            "room_type": "office",
            "asset_name": "office_chair",
            "good_count": 8,
            "bad_count": 8,
            "good_comment": "Office chair keeps usable desk distance and faces the work surface.",
            "bad_comment": "Office chair sits in the room, but desk relation is weak.",
            "good": make_features(
                role_seating=1, wall_closeness=0.43, center_closeness=0.42,
                door_clearance=0.79, window_clearance=0.72, corner_closeness=0.25,
                wall_aligned=1, faces_room_center=0.61, has_table_context=1,
                table_distance_fit=0.94, table_facing_alignment=0.95
            ),
            "bad": make_features(
                role_seating=1, wall_closeness=0.77, center_closeness=0.18,
                door_clearance=0.45, window_clearance=0.55, corner_closeness=0.57,
                wall_aligned=0, faces_room_center=0.22, has_table_context=1,
                table_distance_fit=0.24, table_facing_alignment=0.19
            ),
        },
        {
            "group_id": "bed_bedroom_01",
            "room_type": "bedroom",
            "asset_name": "bed",
            "good_count": 10,
            "bad_count": 10,
            "good_comment": "Bed keeps headboard near wall and open side towards room.",
            "bad_comment": "Bed rotates wrong way or drifts to center and passages.",
            "good": make_features(
                prefer_wall=1, keep_center_free=1, is_large=1, role_anchor=1,
                wall_closeness=0.93, center_closeness=0.14, door_clearance=0.82,
                window_clearance=0.77, corner_closeness=0.48, wall_aligned=1,
                faces_room_center=0.88
            ),
            "bad": make_features(
                prefer_wall=1, keep_center_free=1, is_large=1, role_anchor=1,
                wall_closeness=0.18, center_closeness=0.83, door_clearance=0.34,
                window_clearance=0.29, corner_closeness=0.66, wall_aligned=0,
                faces_room_center=0.12
            ),
        },
        {
            "group_id": "sofa_living_01",
            "room_type": "living_room",
            "asset_name": "sofa",
            "good_count": 8,
            "bad_count": 8,
            "good_comment": "Sofa anchors to wall and faces the interior.",
            "bad_comment": "Sofa blocks circulation and loses semantic facing.",
            "good": make_features(
                prefer_wall=1, keep_center_free=1, is_large=1, role_anchor=1,
                wall_closeness=0.89, center_closeness=0.19, door_clearance=0.81,
                window_clearance=0.71, corner_closeness=0.31, wall_aligned=1,
                faces_room_center=0.91
            ),
            "bad": make_features(
                prefer_wall=1, keep_center_free=1, is_large=1, role_anchor=1,
                wall_closeness=0.25, center_closeness=0.79, door_clearance=0.28,
                window_clearance=0.22, corner_closeness=0.56, wall_aligned=0,
                faces_room_center=0.21
            ),
        },
        {
            "group_id": "wardrobe_bedroom_01",
            "room_type": "bedroom",
            "asset_name": "wardrobe",
            "good_count": 8,
            "bad_count": 8,
            "good_comment": "Wardrobe stays on perimeter and away from openings.",
            "bad_comment": "Wardrobe drifts to center or too close to openings.",
            "good": make_features(
                prefer_wall=1, keep_center_free=1, is_large=1, role_storage=1,
                wall_closeness=0.94, center_closeness=0.11, door_clearance=0.86,
                window_clearance=0.89, corner_closeness=0.46, wall_aligned=1,
                faces_room_center=0.74
            ),
            "bad": make_features(
                prefer_wall=1, keep_center_free=1, is_large=1, role_storage=1,
                wall_closeness=0.13, center_closeness=0.81, door_clearance=0.18,
                window_clearance=0.14, corner_closeness=0.27, wall_aligned=0,
                faces_room_center=0.17
            ),
        },
        {
            "group_id": "dresser_bedroom_01",
            "room_type": "bedroom",
            "asset_name": "dresser",
            "good_count": 8,
            "bad_count": 8,
            "good_comment": "Dresser stands at wall and keeps room center open.",
            "bad_comment": "Dresser occupies the flow zone and sits too close to door or window.",
            "good": make_features(
                prefer_wall=1, keep_center_free=1, role_storage=1,
                wall_closeness=0.88, center_closeness=0.18, door_clearance=0.79,
                window_clearance=0.74, corner_closeness=0.39, wall_aligned=1,
                faces_room_center=0.69
            ),
            "bad": make_features(
                prefer_wall=1, keep_center_free=1, role_storage=1,
                wall_closeness=0.26, center_closeness=0.76, door_clearance=0.21,
                window_clearance=0.23, corner_closeness=0.19, wall_aligned=0,
                faces_room_center=0.16
            ),
        },
        {
            "group_id": "dining_table_01",
            "room_type": "dining_room",
            "asset_name": "dining_table",
            "good_count": 8,
            "bad_count": 8,
            "good_comment": "Dining table stays near useful center and clear from openings.",
            "bad_comment": "Dining table gets stuck near wall and loses group usability.",
            "good": make_features(
                role_table=1, room_dining_or_kitchen=1, wall_closeness=0.32,
                center_closeness=0.83, door_clearance=0.82, window_clearance=0.76,
                corner_closeness=0.21, wall_aligned=1, faces_room_center=0.58
            ),
            "bad": make_features(
                role_table=1, room_dining_or_kitchen=1, wall_closeness=0.89,
                center_closeness=0.16, door_clearance=0.38, window_clearance=0.29,
                corner_closeness=0.52, wall_aligned=0, faces_room_center=0.11
            ),
        },
        {
            "group_id": "coffee_table_01",
            "room_type": "living_room",
            "asset_name": "coffee_table",
            "good_count": 8,
            "bad_count": 8,
            "good_comment": "Coffee table stays accessible and roughly central to the seating group.",
            "bad_comment": "Coffee table sticks to perimeter and stops acting as group center.",
            "good": make_features(
                role_table=1, center_closeness=0.74, wall_closeness=0.27,
                door_clearance=0.81, window_clearance=0.73, corner_closeness=0.18,
                wall_aligned=1, faces_room_center=0.57
            ),
            "bad": make_features(
                role_table=1, center_closeness=0.14, wall_closeness=0.91,
                door_clearance=0.41, window_clearance=0.35, corner_closeness=0.61,
                wall_aligned=0, faces_room_center=0.18
            ),
        },
        {
            "group_id": "sink_bathroom_01",
            "room_type": "bathroom",
            "asset_name": "sink",
            "good_count": 8,
            "bad_count": 8,
            "good_comment": "Sink stays on perimeter and keeps center available.",
            "bad_comment": "Sink sits in circulation path or too close to opening.",
            "good": make_features(
                prefer_wall=1, keep_center_free=1, role_anchor=1, room_bathroom=1,
                wall_closeness=0.93, center_closeness=0.12, door_clearance=0.68,
                window_clearance=0.71, corner_closeness=0.41, wall_aligned=1,
                faces_room_center=0.77
            ),
            "bad": make_features(
                prefer_wall=1, keep_center_free=1, role_anchor=1, room_bathroom=1,
                wall_closeness=0.14, center_closeness=0.85, door_clearance=0.16,
                window_clearance=0.18, corner_closeness=0.24, wall_aligned=0,
                faces_room_center=0.15
            ),
        },
        {
            "group_id": "toilet_bathroom_01",
            "room_type": "bathroom",
            "asset_name": "toilet",
            "good_count": 8,
            "bad_count": 8,
            "good_comment": "Toilet stays on perimeter with safe opening clearance.",
            "bad_comment": "Toilet rotates into the room and hurts circulation.",
            "good": make_features(
                prefer_wall=1, keep_center_free=1, role_anchor=1, room_bathroom=1,
                wall_closeness=0.95, center_closeness=0.11, door_clearance=0.72,
                window_clearance=0.78, corner_closeness=0.36, wall_aligned=1,
                faces_room_center=0.73
            ),
            "bad": make_features(
                prefer_wall=1, keep_center_free=1, role_anchor=1, room_bathroom=1,
                wall_closeness=0.16, center_closeness=0.79, door_clearance=0.17,
                window_clearance=0.22, corner_closeness=0.26, wall_aligned=0,
                faces_room_center=0.14
            ),
        },
        {
            "group_id": "washing_machine_01",
            "room_type": "laundry_room",
            "asset_name": "washing_machine",
            "good_count": 8,
            "bad_count": 8,
            "good_comment": "Laundry appliance stays at wall and leaves circulation free.",
            "bad_comment": "Laundry appliance occupies center or opening keepout.",
            "good": make_features(
                prefer_wall=1, keep_center_free=1, role_storage=1,
                wall_closeness=0.91, center_closeness=0.15, door_clearance=0.81,
                window_clearance=0.74, corner_closeness=0.35, wall_aligned=1,
                faces_room_center=0.66
            ),
            "bad": make_features(
                prefer_wall=1, keep_center_free=1, role_storage=1,
                wall_closeness=0.19, center_closeness=0.77, door_clearance=0.22,
                window_clearance=0.18, corner_closeness=0.17, wall_aligned=0,
                faces_room_center=0.18
            ),
        },
        {
            "group_id": "fridge_kitchen_01",
            "room_type": "kitchen",
            "asset_name": "fridge",
            "good_count": 8,
            "bad_count": 8,
            "good_comment": "Fridge behaves as perimeter storage with safe opening distance.",
            "bad_comment": "Fridge sits in central area and blocks use paths.",
            "good": make_features(
                prefer_wall=1, keep_center_free=1, is_large=1, role_storage=1,
                room_dining_or_kitchen=1, wall_closeness=0.93, center_closeness=0.13,
                door_clearance=0.83, window_clearance=0.76, corner_closeness=0.44,
                wall_aligned=1, faces_room_center=0.72
            ),
            "bad": make_features(
                prefer_wall=1, keep_center_free=1, is_large=1, role_storage=1,
                room_dining_or_kitchen=1, wall_closeness=0.22, center_closeness=0.72,
                door_clearance=0.29, window_clearance=0.24, corner_closeness=0.31,
                wall_aligned=0, faces_room_center=0.19
            ),
        },
        {
            "group_id": "bookshelf_01",
            "room_type": "living_room",
            "asset_name": "bookshelf",
            "good_count": 8,
            "bad_count": 8,
            "good_comment": "Bookshelf behaves like storage and keeps perimeter alignment.",
            "bad_comment": "Bookshelf floats in center where it looks implausible.",
            "good": make_features(
                prefer_wall=1, keep_center_free=1, role_storage=1,
                wall_closeness=0.92, center_closeness=0.14, door_clearance=0.79,
                window_clearance=0.82, corner_closeness=0.49, wall_aligned=1,
                faces_room_center=0.71
            ),
            "bad": make_features(
                prefer_wall=1, keep_center_free=1, role_storage=1,
                wall_closeness=0.15, center_closeness=0.84, door_clearance=0.43,
                window_clearance=0.28, corner_closeness=0.18, wall_aligned=0,
                faces_room_center=0.16
            ),
        },
        {
            "group_id": "desk_office_01",
            "room_type": "office",
            "asset_name": "desk",
            "good_count": 8,
            "bad_count": 8,
            "good_comment": "Desk keeps usable wall relation and open working side.",
            "bad_comment": "Desk turns away from the room and loses workstation logic.",
            "good": make_features(
                prefer_wall=1, role_table=1, wall_closeness=0.74,
                center_closeness=0.34, door_clearance=0.82, window_clearance=0.74,
                corner_closeness=0.37, wall_aligned=1, faces_room_center=0.84
            ),
            "bad": make_features(
                prefer_wall=1, role_table=1, wall_closeness=0.41,
                center_closeness=0.59, door_clearance=0.39, window_clearance=0.31,
                corner_closeness=0.22, wall_aligned=0, faces_room_center=0.17
            ),
        },
        {
            "group_id": "armchair_living_01",
            "room_type": "living_room",
            "asset_name": "armchair",
            "good_count": 8,
            "bad_count": 8,
            "good_comment": "Armchair stays in seating zone and faces room activity.",
            "bad_comment": "Armchair gets stranded near perimeter with poor facing.",
            "good": make_features(
                role_seating=1, wall_closeness=0.42, center_closeness=0.55,
                door_clearance=0.81, window_clearance=0.73, corner_closeness=0.31,
                wall_aligned=1, faces_room_center=0.79
            ),
            "bad": make_features(
                role_seating=1, wall_closeness=0.89, center_closeness=0.17,
                door_clearance=0.33, window_clearance=0.27, corner_closeness=0.54,
                wall_aligned=0, faces_room_center=0.19
            ),
        },
        {
            "group_id": "nightstand_bedroom_01",
            "room_type": "bedroom",
            "asset_name": "nightstand",
            "good_count": 8,
            "bad_count": 8,
            "good_comment": "Nightstand uses a corner-like perimeter position near a bed zone.",
            "bad_comment": "Nightstand in center loses its secondary supportive role.",
            "good": make_features(
                prefer_wall=1, prefer_corner=1, keep_center_free=1, role_storage=1,
                wall_closeness=0.87, center_closeness=0.22, door_clearance=0.76,
                window_clearance=0.73, corner_closeness=0.86, wall_aligned=1,
                faces_room_center=0.58
            ),
            "bad": make_features(
                prefer_wall=1, prefer_corner=1, keep_center_free=1, role_storage=1,
                wall_closeness=0.21, center_closeness=0.83, door_clearance=0.41,
                window_clearance=0.36, corner_closeness=0.08, wall_aligned=0,
                faces_room_center=0.18
            ),
        },
    ]

    sample_no = 0
    for scenario in scenarios:
        for label_name, label_value, base_name, count, comment in (
            ("good", 1.0, "good", scenario["good_count"], scenario["good_comment"]),
            ("bad", 0.0, "bad", scenario["bad_count"], scenario["bad_comment"]),
        ):
            base_features = scenario[base_name]
            for _ in range(count):
                sample_no += 1
                samples.append(
                    {
                        "sample_id": f"{scenario['group_id']}_{label_name}_{sample_no:04d}",
                        "group_id": scenario["group_id"],
                        "room_type": scenario["room_type"],
                        "asset_name": scenario["asset_name"],
                        "label": label_value,
                        "comment": comment,
                        "features": jitter_features(base_features, rng),
                    }
                )

    return {
        "version": 2,
        "description": (
            "Binary candidate-quality dataset for TAM layout solver. "
            "Samples are expert-shaped local furniture placements used to train "
            "a real linear classifier that predicts how human-like one candidate looks."
        ),
        "feature_order": FEATURE_ORDER,
        "samples": samples,
    }
""" End of 'generate_dataset' function """

"""Convert one dataset sample to fixed-order vector.
* ARGUMENTS:
*   - sample:
*       dict dataset record object;
* RETURNS:
*   (list) floating-point vector ordered by global features list.
"""
def feature_vector(sample: Dict[str, object]) -> List[float]:
    features = sample["features"]
    return [float(features[name]) for name in FEATURE_ORDER]
""" End of 'feature_vector' function """

"""Split dataset into train and validation subsets with label balance.
* ARGUMENTS:
*   - samples:
*       list collection of dataset sample objects;
*   - seed:
*       int generation initialization value for mixing arrays;
* RETURNS:
*   (tuple) balanced sub-collections for training and verification.
"""
def split_dataset(samples: List[Dict[str, object]], seed: int) -> Tuple[List[Dict[str, object]], List[Dict[str, object]]]:
    rng = random.Random(seed)
    positive = [sample for sample in samples if float(sample["label"]) >= 0.5]
    negative = [sample for sample in samples if float(sample["label"]) < 0.5]

    rng.shuffle(positive)
    rng.shuffle(negative)

    positive_cut = max(1, int(len(positive) * 0.8))
    negative_cut = max(1, int(len(negative) * 0.8))

    train = positive[:positive_cut] + negative[:negative_cut]
    valid = positive[positive_cut:] + negative[negative_cut:]
    rng.shuffle(train)
    rng.shuffle(valid)
    return train, valid
""" End of 'split_dataset' function """

"""Compute z-score normalization for features.
* ARGUMENTS:
*   - vectors:
*       list collection of raw quantitative feature configurations;
* RETURNS:
*   (tuple) calculated mean balances, variances and adjusted configurations.
"""
def compute_normalization(vectors: List[List[float]]) -> Tuple[List[float], List[float], List[List[float]]]:
    count = len(vectors)
    feature_count = len(vectors[0])
    mean = [0.0] * feature_count
    scale = [1.0] * feature_count
    normalized: List[List[float]] = []

    for feature_no in range(feature_count):
        mean[feature_no] = sum(vector[feature_no] for vector in vectors) / max(1, count)

    for feature_no in range(feature_count):
        variance = sum((vector[feature_no] - mean[feature_no]) ** 2 for vector in vectors) / max(1, count)
        scale[feature_no] = math.sqrt(variance)
        if scale[feature_no] < 1e-6:
            scale[feature_no] = 1.0

    for vector in vectors:
        normalized.append([(vector[i] - mean[i]) / scale[i] for i in range(feature_count)])
    return mean, scale, normalized
""" End of 'compute_normalization' function """

"""Stable logistic sigmoid.
* ARGUMENTS:
*   - value:
*       float quantitative activation power representation;
* RETURNS:
*   (float) target normalized probability score within range [0; 1].
"""
def sigmoid(value: float) -> float:
    if value >= 0.0:
        exponent = math.exp(-value)
        return 1.0 / (1.0 + exponent)
    exponent = math.exp(value)
    return exponent / (1.0 + exponent)
""" End of 'sigmoid' function """

"""Compute dot product.
* ARGUMENTS:
*   - lhs:
*       list left operational sequence vectors;
*   - rhs:
*       list right operational sequence vectors;
* RETURNS:
*   (float) summary scalar product calculation results.
"""
def dot(lhs: List[float], rhs: List[float]) -> float:
    return sum(a * b for a, b in zip(lhs, rhs))
""" End of 'dot' function """

"""Evaluate binary classifier on one split.
* ARGUMENTS:
*   - weights:
*       list trained linear function scaling items;
*   - bias:
*       float trained offset coefficient parameter;
*   - vectors:
*       list normalized numeric validation sequences;
*   - labels:
*       list binary control verification array;
* RETURNS:
*   (dict) computed target error and categorical accuracy properties.
"""
def evaluate_dataset(weights: List[float], bias: float, vectors: List[List[float]], labels: List[float]) -> Dict[str, float]:
    loss = 0.0
    correct = 0

    for vector, label in zip(vectors, labels):
        probability = sigmoid(dot(weights, vector) + bias)
        probability = min(max(probability, 1e-6), 1.0 - 1e-6)
        loss += -(label * math.log(probability) + (1.0 - label) * math.log(1.0 - probability))
        if (probability >= 0.5) == (label >= 0.5):
            correct += 1

    count = max(1, len(labels))
    return {
        "loss": loss / count,
        "accuracy": correct / count,
    }
""" End of 'evaluate_dataset' function """

"""Train real linear logistic regression with L2 regularization.
* ARGUMENTS:
*   - train_vectors:
*       list target numeric baseline sequences for optimization;
*   - train_labels:
*       list structural reference objective layout assessments;
* RETURNS:
*   (tuple) balanced weight configurations and model offset variable.
"""
def train_logistic_model(train_vectors: List[List[float]], train_labels: List[float]) -> Tuple[List[float], float]:
    feature_count = len(train_vectors[0])
    weights = [0.0] * feature_count
    bias = 0.0
    learning_rate = 0.09
    regularization = 0.0008
    epoch_count = 3200
    count = len(train_vectors)

    for _ in range(epoch_count):
        gradient_w = [0.0] * feature_count
        gradient_b = 0.0

        for vector, label in zip(train_vectors, train_labels):
            probability = sigmoid(dot(weights, vector) + bias)
            error = probability - label

            for feature_no in range(feature_count):
                gradient_w[feature_no] += error * vector[feature_no]
            gradient_b += error

        for feature_no in range(feature_count):
            gradient_w[feature_no] = gradient_w[feature_no] / count + regularization * weights[feature_no]
            weights[feature_no] -= learning_rate * gradient_w[feature_no]
        bias -= learning_rate * (gradient_b / count)

    return weights, bias
""" End of 'train_logistic_model' function """

"""Write one JSON file with deterministic formatting.
* ARGUMENTS:
*   - file_name:
*       Path relative target operating system workspace address;
*   - data:
*       dict nested project entities intended for serialization;
* RETURNS:
*   None.
"""
def save_json(file_name: Path, data: Dict[str, object]) -> None:
    file_name.parent.mkdir(parents=True, exist_ok=True)
    file_name.write_text(json.dumps(data, indent=2, ensure_ascii=False) + "\n", encoding="utf-8")
""" End of 'save_json' function """

"""Train real linear layout quality model.
* ARGUMENTS:
*   None.
* RETURNS:
*   (int) operating system process termination code signal.
"""
def main() -> int:
    parser = argparse.ArgumentParser(description="Train real linear layout quality model.")
    parser.add_argument("--seed", type=int, default=42, help="Deterministic random seed.")
    parser.add_argument("--dataset", type=Path, default=None, help="Optional dataset file path.")
    parser.add_argument("--model", type=Path, default=None, help="Optional model file path.")
    parser.add_argument("--rebuild-dataset", action="store_true", help="Regenerate synthetic expert-labeled dataset.")
    args = parser.parse_args()

    script_dir = Path(__file__).resolve().parent
    project_root = script_dir.parent
    dataset_file = args.dataset or (project_root / "bin" / "layout_ai" / "layout_rank_dataset.json")
    model_file = args.model or (project_root / "bin" / "layout_ai" / "layout_rank_model.json")

    if args.rebuild_dataset or not dataset_file.exists():
        dataset_root = generate_dataset(args.seed)
        save_json(dataset_file, dataset_root)
    else:
        dataset_root = json.loads(dataset_file.read_text(encoding="utf-8"))

    samples = dataset_root["samples"]
    train_samples, valid_samples = split_dataset(samples, args.seed)

    train_raw = [feature_vector(sample) for sample in train_samples]
    valid_raw = [feature_vector(sample) for sample in valid_samples]
    train_labels = [float(sample["label"]) for sample in train_samples]
    valid_labels = [float(sample["label"]) for sample in valid_samples]

    feature_mean, feature_scale, train_vectors = compute_normalization(train_raw)
    valid_vectors = [
        [(vector[i] - feature_mean[i]) / feature_scale[i] for i in range(len(FEATURE_ORDER))]
        for vector in valid_raw
    ]

    weights, bias = train_logistic_model(train_vectors, train_labels)
    train_metrics = evaluate_dataset(weights, bias, train_vectors, train_labels)
    valid_metrics = evaluate_dataset(weights, bias, valid_vectors, valid_labels)

    model_root = {
        "version": 1,
        "model_type": "linear_logistic_candidate_quality",
        "dataset_file": str(dataset_file.name),
        "feature_order": FEATURE_ORDER,
        "feature_count": len(FEATURE_ORDER),
        "sample_count": len(samples),
        "train_sample_count": len(train_samples),
        "validation_sample_count": len(valid_samples),
        "feature_mean": feature_mean,
        "feature_scale": feature_scale,
        "weights": weights,
        "bias": bias,
        "train_metrics": train_metrics,
        "validation_metrics": valid_metrics,
    }
    save_json(model_file, model_root)

    print(f"Dataset samples: {len(samples)}")
    print(f"Train samples:   {len(train_samples)}")
    print(f"Valid samples:   {len(valid_samples)}")
    print(f"Train loss:      {train_metrics['loss']:.6f}")
    print(f"Train accuracy:  {train_metrics['accuracy']:.4f}")
    print(f"Valid loss:      {valid_metrics['loss']:.6f}")
    print(f"Valid accuracy:  {valid_metrics['accuracy']:.4f}")
    print(f"Dataset file:    {dataset_file}")
    print(f"Model file:      {model_file}")
    return 0
""" End of 'main' function """

if __name__ == "__main__":
    raise SystemExit(main())


""" END OF 'train_layout_rank_model.py' FILE """
