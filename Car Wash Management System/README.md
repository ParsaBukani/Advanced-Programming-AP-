
# Car Wash Management System

A command-line application simulating a **car wash operation**, developed in **C++** using **object-oriented design** principles. The program models workers, washing stages, customers, and time-driven car servicing logic.

## Overview

Customers arrive at the car wash with a custom list of washing stages. Each car moves through stages in order, managed by available workers. If no suitable worker is free, cars enter a queue for that stage.

Key components of the system:

-   **Cars** move sequentially through wash stages.
    
-   **Workers** are assigned based on availability, efficiency, and ID priority.
    
-   **Stages** have capacities and prices; income is tracked per stage.
    
-   **Time** is simulated linearly and events are logged at each step.
    

## 📄 Commands

-   `pass_time <N>` – Advance time by N units and print event log

-   `car_arrival <stageIDs>` – Add a new car with selected stages
    
-   `get_car_status <carID>` – Print status of a car
    
-   `get_stage_status <stageID>` – Print stats and income of a stage
    
-   `get_worker_status <workerID>` – Show worker status
    

## 📁 Input Format

-   `Stages.csv`: Defines wash stages and their prices
    
-   `Workers.csv`: Worker assignment, stage, and wash durations
    

## License

This project is licensed under the **MIT License**.

## Acknowledgements

Developed under the supervision of **Dr. Ramtin Khosravi**  
Designed by Ali Hamzehpoor, Hesam Ramazanian, Soroosh Sahraei,  
Ali Ataollahi, Shahnam Feyzian, Amirreza Nadi, Mohammadamin Yousefi
