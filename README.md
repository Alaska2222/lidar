# Lidar
This is my custom solution for Lidar robot localization problem

# LiDAR Robot Localization

## Overview

**Constraints**: 10 seconds, 512 MB

This project involves processing log data from a robot equipped with a 2D LiDAR sensor, which moves within an indoor environment. The goal is to determine the robot's final coordinates after completing its movement and calculate its average speed. The robot operates exclusively on a floor (2D space), changing its coordinates while maintaining its orientation. 

The robot is equipped with a single sensor—the 2D LiDAR—capable of scanning the surroundings. The scanning process works as follows: the LiDAR emits **K** rays, with an angle of **360/K** degrees between adjacent rays. For each ray, the distance to the nearest wall in that direction is measured. 

### LiDAR Specifications

- The LiDAR has a distance measurement error of **Sl**, which is modeled as a normally distributed random variable with a standard deviation of **Sl**. 
- There is no angular error.
- Data from the LiDAR is received in ascending order based on the angle, starting with the measurement along the X-axis.
- Scanning occurs instantaneously.

### Robot Movement

The robot's movement is defined as a shift along a vector (X, Y) with standard deviations along each axis (Sx, Sy). Movements along the X and Y axes are independent. The robot's lifecycle consists of alternating between scanning and moving.

### Environment

The environment is represented as a non-self-intersecting polygon.

## Test Examples

- [Test Example 1](https://drive.google.com/file/d/1oKJG0GzDMqiUCQ1ETId_lzJM9FVppJST/view?usp=sharing)
  - Output: `2.99802 3.03753`
  
- [Test Example 2](https://drive.google.com/file/d/1vY0ifjkmSohlEPi31H95I2mYPEU4Mf4m/view?usp=sharing)
  - Output: `1.9406 8.00725`
  
- [Test Example 3](https://drive.google.com/file/d/19xgVFV5x9-4pigDZ-E7YT7gIMOYlG_hb/view?usp=sharing)
  - Output: `1.59266 8.63027`
  
- [Test Example 4](https://drive.google.com/file/d/1CV8pNotTxMZ6HYmOuOL9WuYNE5Ck40Nu/view?usp=sharing)
  - Output: `3.62952 15.1485`

## Input Data Format

1. The first line contains the integer **N** - the number of sides of the room.
2. The next line contains **N** pairs of integers representing the coordinates (X, Y) of the room.
3. The following line contains integers **M** (number of movements of the robot) and **K** (number of LiDAR rays).
4. The next line contains three floating-point numbers: **Sl**, **Sx**, **Sy** (the measurement errors).
5. The next line contains either **1** or **0**:
   - If **1**, it is followed by two floating-point numbers—**x** and **y**—indicating the robot's initial coordinates.
   - If **0**, the initial position is unknown.
6. Following this, there are **M** pairs of lines describing the robot's scans and movements:
   - The first line of each pair contains **K** floating-point numbers representing LiDAR measurements.
   - The second line contains two floating-point numbers indicating the robot's movement.
   - The last line contains **K** floating-point numbers representing the LiDAR measurements after the movement.

## Output Data Format

Output one line with two numbers representing the robot's coordinates after it has finished moving.

## Constraints

- \(3 \leq N \leq 100\)
- \(-1000 \leq X, Y \leq 1000\)
- \(0 \leq M \leq 1000\)
- \(36 \leq K \leq 3600\)
- \(0.001 \leq Sl, Sx, Sy \leq 0.2\)
- The area of the room must be at least 0.1 times the area of the rectangle surrounding it.


