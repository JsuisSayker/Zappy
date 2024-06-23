# 🎮 • Zappy

Zappy is Epitech's second-year final project.<br><br>This project **aimed at developing a network game** where multiple **AI teams** compete on a field to gather resources.<br>
The **winning team** is the one that manages to have at least **6 players reaching the maximum level.**

## Table of Contents

1. [**General Info**](#general-info)
2. [**Project Components**](#project-components)
3. [**Installation**](#installation)
3. [**Usage**](#usage)
4. [**Trailer**](#trailer)

## General Info

Players, represented by AIs, move around a field to gather necessary resources to evolve.<br>
Each player starts at level 1 and must collect specific resources to reach higher levels. The ultimate goal is to have at least 6 team members reaching the maximum level.

Teams need to devise strategies to optimize resource collection and hinder other teams' progress. Communication and coordination among AIs within the same team are crucial to achieving this goal.

## Project Components

### Server
- **Author:**
  - [**MENDY Tom**](https://github.com/Tom-Mendy)
  - [**MENDIA Marc**](https://github.com/Dvaking)
- **Technology:**<br>
&emsp;&emsp;&emsp;[![C](https://img.shields.io/badge/C-logo?style=for-the-badge&logo=C&logoColor=white&color=%23A8B9CC)](https://fr.wikipedia.org/wiki/C_(langage))<br>
- **Features:**
  - **Manages communication** between GUI and AIs
  - **Manages resources** and field state
  - **Synchronizes and manages** AI and GUI actions

### Artificial Intelligences (AIs)
- **Author:**
  - [**TROUVE Killian**](https://github.com/JsuisSayker)
- **Technology:**<br>
&emsp;&emsp;&emsp;[![Python](https://img.shields.io/badge/python-logo?style=for-the-badge&logo=Python&logoColor=white&color=%233776AB)](https://docs.python.org/3/)
&ensp;[![Numpy](https://img.shields.io/badge/numyp-logo?style=for-the-badge&logo=Numpy&color=%23013243)](https://numpy.org/doc/stable/)
- **Features:**
  - **Resource gathering** strategy
  - **Autonomous decision-making** to reach the maximum level
  - **Coordination** with other team members

### Graphical Client (GUI)
- **Author:**
  - [**CHLODNIK Aléa**](https://github.com/AleaChlodnik)
  - [**GAUFFRE Arthur**](https://github.com/arthurgauffre)
  - [**GIRAUD Romain**](https://github.com/OxiiLB)
- **Technology:**<br>
&emsp;&emsp;&emsp;[![C++](https://img.shields.io/badge/C%2B%2B-logo?style=for-the-badge&logo=C%2B%2B&logoColor=white&color=%2300599C)](https://fr.wikipedia.org/wiki/C%2B%2B)
&ensp;[![Vulkan](https://img.shields.io/badge/vulkan-logo?style=for-the-badge&logo=Vulkan&logoColor=white&color=%23AC162C)](https://www.vulkan.org/learn)
- **Features:**
  - Real-time visualization of the field and AI actions
  - User interface to monitor team progress
  - Debug and performance monitoring tools

## Installation

## Usage
**After installation**, here's how to launch the project
### Server Launch:

```
   ./zappy_server -p [PORT] -x [WIDTH] -y [HEIGHT] -n [TEAMS] -c [NB_CLIENTS] -f [FREQ]

      -p:           Connection port to the server.
      -x & h:       Dimensions of the game map.
      -n:           Names of all the teams.
      -c:           Maximum number of players per team.
      -f:           Execution frequency of the server.
```

### AIs Launch:
```
   ./zappy_ai -p [PORT] -n [TEAM NAME] -h [SERVER HOSTNAME] -i [ID]

      -p:           Connection port to the server.
      -n:           Team name of this AI.
      -h:           IP address for server connection.
      -i:           Client ID. (optional)
```

### GUI Launch:
```
   ./zappy_gui -p [PORT] -h [SERVER HOSTNAME]

      -p:           Connection port to the server.
      -h:           IP address for server connection.
```

## Trailer

