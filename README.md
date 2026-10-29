# TIVIDY - Generic Workflow Management System

TIVIDY is a C++11 workflow management system built using standard Gang of Four (GoF) design patterns. It coordinates task lifecycles, assignment strategies, escalation/approval handler chains, and multi-level undo operations across diverse domain models.

The engine natively demonstrates two distinct industry pipelines:
1. **Incident Response (Power Outage)**
2. **Job Application (Tech Company)**

---

## 1. System Requirements & Dependencies

The dependencies required to build and verify TIVIDY mirror the packages installed in the project `Dockerfile`:

### Requirements:
* **Operating System**: Linux (Ubuntu 20.04 recommended), macOS or Windows via WSL2 / Docker.
* **C++ Compiler**: `g++` with support for `-std=c++11`, `-Wall` and `-Werror`.
* **Build System**: GNU Make 4.0+ (`make`), CMake 3.10+.
* **Testing Framework**: GoogleTest `release-1.12.1` built with C++11 constraints. (The latest version of GoogleTest requires C++14 or later so we use an older verson of GoogleTest)
* **Memory Checking**: `valgrind`
* **Documentation Engine (Optional)**: `doxygen`
* **System Utilities**: `git`, `ca-certificates` and `pthread` 

---

## 2. Instructions (Docker Highly Recommended)

```bash
# Clone the repository onto your machine
git clone https://github.com/HaythamP/TIVIDY.git

# Ensure you are in the main branch by running
git checkout main

# Build the Docker Image
docker build -t tividy-ci:latest .

# Run Unit Tests
docker run --rm tividy-ci:latest make test-unit

# Run Integration Tests
docker run --rm tividy-ci:latest make test-integration

# Run End to End Tests
docker run --rm tividy-ci:latest make test-e2e

# Run Valgrind
docker run --rm tividy-ci:latest make valgrind

# Stop Container
docker stop <tividy-ci:latest>
```

## Quick Links !remember to put links as the URL!
View the [Demo Slides]()  
View the Final [Report]()  
View the Final [Activity Diagram 1]()  
View the Final [Activity Diagram 2]()  
View the Final [Activity Diagram 3]()  
View the Final [State Diagram]()  
View the Final [Sequence Diagram 1]()  
View the Final [Sequence Diagram 2]()  
View the Final [UML Class Diagram]()  
View the [src](src) folder  
View the [testing](testing) folder  

---
| Profile | Key Contributions |
|:---:|---|
| <img src="assets/rayhaan_object_diagram.jpg" width="450" height="150%" alt="Rayhaan Ahmed"> | Activity Diagram 3, GUI, Implementation: Command, Memento |
| <img src="assets/tashil_object_diagram.jpg" width="450" height="150%" alt="Tashil Haripersad"> |Report, Demo slides, Class Diagram, Implementation: Strategy, Chain of Responsibility, Façade, main |
| <img src="assets/kyle_object_diagram.jpg" width="450" height="150%" alt="Kyle Naicker"> |Activity Diagram 1, State Diagram, Additional diagrams: 2 Object Diagrams and 1 Communication Diagram, Docker, Implementation: State, Iterator, Visitor|
| <img src="assets/haytham_object_diagram.jpg" width="450" height="150%" alt="Haytham Phillips"> |Activity Diagram 2, Sequence Diagram 2, CI/CD pipeline, Integration tests and end-to-end tests, Implementation: Builder, Mediator, exception classes|
| <img src="assets/oratile_object_diagram.jpg" width="450" height="150%" alt="Oratile Lesedi Rabotapi"> |Sequence Diagram 1, GUI, Implementation: Composite, Decorator, Prototype |

All members contributed the header and source files of their own patterns, including the Doxygen comments and unit tests.
### Built With

![C++](https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![GoogleTest](https://img.shields.io/badge/GoogleTest-00599C?style=for-the-badge&logo=googletest&logoColor=white)
![Docker](https://img.shields.io/badge/docker-%230db7ed.svg?style=for-the-badge&logo=docker&logoColor=white)
![GitHub Actions](https://img.shields.io/badge/github%20actions-%232671E5.svg?style=for-the-badge&logo=githubactions&logoColor=white)
![Ubuntu](https://img.shields.io/badge/Ubuntu-E95420?style=for-the-badge&logo=ubuntu&logoColor=white)
![Git](https://img.shields.io/badge/git-%23F05033.svg?style=for-the-badge&logo=git&logoColor=white)
![GNU Bash](https://img.shields.io/badge/GNU%20Bash-4EAA25?style=for-the-badge&logo=GNU%20Bash&logoColor=white)
![Makefile](https://img.shields.io/badge/Makefile-064F8C?style=for-the-badge&logo=gnu&logoColor=white)
![HTML5](https://img.shields.io/badge/HTML5-E34F26?style=for-the-badge&logo=html5&logoColor=white)
![CSS3](https://img.shields.io/badge/CSS3-1572B6?style=for-the-badge&logo=css3&logoColor=white)
![JavaScript](https://img.shields.io/badge/JavaScript-F7DF1E?style=for-the-badge&logo=javascript&logoColor=black)
![JSON](https://img.shields.io/badge/JSON-000000?style=for-the-badge&logo=json&logoColor=white)