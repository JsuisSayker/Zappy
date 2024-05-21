#!/usr/bin/env python3
import argparse


class OrderNamespace(argparse.Namespace):
    def __init__(self, **kwargs):
        self.__dict__["order"] = []
        super(OrderNamespace, self).__init__(**kwargs)

    def __setattr__(self, attr, value):
        self.__dict__["order"].append((attr, value))
        super(OrderNamespace, self).__setattr__(attr, value)


parser = argparse.ArgumentParser(
    prog="zappy_ai",
    description="an AI for the Zappy project",
    usage="./zappy_ai -p port -n name -h machine",
    add_help=False,
)

parser.add_argument(
    "-p",
    metavar="port",
    nargs=1,
    help="port number",
)

parser.add_argument(
    "-n",
    metavar="name",
    nargs=1,
    help="name of the team",
)

parser.add_argument(
    "-h",
    metavar="machine",
    nargs=1,
    help="name of the machine",
)

try:
    args = parser.parse_args(args=None, namespace=OrderNamespace())
except SystemExit as e:
    print(f"error {e.code}, {e.args}")
    exit(84)
