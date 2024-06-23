# 🎮 • Zappy

Zappy is Epitech's second-year final project.

This project **aimed at developing a network game** where multiple **AI teams** compete on a field to gather resources.

The **winning team** is the one that manages to have at least **6 players reaching the maximum level.**

## Table of Contents

1. [**General Info**](#general-info)
2. [**Project Components**](#project-components)
3. [**Installation**](#installation)
4. [**Usage**](#usage)
5. [**Trailer**](#trailer)

## General Info

Players, represented by AIs, move around a field to gather necessary resources to evolve.

Each player starts at level 1 and must collect specific resources to reach higher levels. The ultimate goal is to have at least 6 team members reaching the maximum level.

Teams need to devise strategies to optimize resource collection and hinder other teams' progress. Communication and coordination among AIs within the same team are crucial to achieving this goal.

## Project Components

### Server

- **Author:**
  - [**MENDY Tom**](https://github.com/Tom-Mendy)
  - [**MENDIA Marc**](https://github.com/Dvaking)
- **Technology:**
&emsp;&emsp;&emsp;[![C](https://img.shields.io/badge/C-logo?style=for-the-badge&logo=C&logoColor=white&color=%23A8B9CC)](https://fr.wikipedia.org/wiki/C_(langage))
- **Features:**
  - **Manages communication** between GUI and AIs
  - **Manages resources** and field state
  - **Synchronizes and manages** AI and GUI actions

### Artificial Intelligences (AIs)

- **Author:**
  - [**TROUVE Killian**](https://github.com/JsuisSayker)
- **Technology:**
&emsp;&emsp;&emsp;[![Python](https://img.shields.io/badge/python-logo?style=for-the-badge&logo=Python&logoColor=white&color=%233776AB)](https://docs.python.org/3/) [![Numpy](https://img.shields.io/badge/numyp-logo?style=for-the-badge&logo=Numpy&color=%23013243)](https://numpy.org/doc/stable/)
- **Features:**
  - **Resource gathering** strategy
  - **Autonomous decision-making** to reach the maximum level
  - **Coordination** with other team members

### Graphical Client (GUI)

- **Author:**
  - [**CHLODNIK Aléa**](https://github.com/AleaChlodnik)
  - [**GAUFFRE Arthur**](https://github.com/arthurgauffre)
  - [**GIRAUD Romain**](https://github.com/OxiiLB)
- **Technology:**
&emsp;&emsp;&emsp;[![C++](https://img.shields.io/badge/C%2B%2B-logo?style=for-the-badge&logo=C%2B%2B&logoColor=white&color=%2300599C)](https://fr.wikipedia.org/wiki/C%2B%2B) [![Vulkan](https://img.shields.io/badge/vulkan-logo?style=for-the-badge&logo=Vulkan&logoColor=white&color=%23AC162C)](https://www.vulkan.org/learn)
- **Features:**
  - Real-time visualization of the field and AI actions
  - User interface to monitor team progress
  - Debug and performance monitoring tools

## Installation

To install gui dependencies, do this once:

```sh
./InstallGuiDependencies.sh
```

To install AI dependencies, do this once:

```sh
pip install numpy
```

To make the project:

```sh
make
```

## Usage

**After installation**, here's how to launch the project

### Launch Server

```sh
./zappy_server -p port -x width -y height -n name1 name2 ... -c clientsNb -f freq
        port            is the port number
        width           is the width of the world
        height          is the height of the world
        nameX           is the name of the team X
        clientsNb       is the number of authorized clients per team
        freq            is the reciprocal of time unit for execution of actions
```

### Launch GUI

```sh
./zappy_gui -p port -h hostname
        port            is the port number
        hostname        is the server address (localhost = 127.0.0.1)
```

### Launch AI

```sh
./zappy_ai -p port -n name -h machine
        port            is the port number
        name            is the name of the team
        hostname        is the server address (localhost = 127.0.0.1)
```

## Trailer
