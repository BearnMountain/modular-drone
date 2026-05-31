Quadcopter for stability with a focus on aerodynamic efficiency, weight savings, and a robust long range link. 

Key Design Elements:
- **Frame Size & Geometry**: 7in frame creates more lift at lower RPM, increasing aerodynamic efficiency and flight times
- **Motors**: 1300kV motor with 6S battery to draw fewer amps, resulting in extended cruising time and reduced motor heating
- **Propellers**: 7in, two-blade for better thrust and efficiency
- **Battery**: Li-ion(durable and high energy storage, superior than LiPos high light, fast discharging which is better for FPV drones) battery with 6S 6000mAh
- **Radio Links**: 
	- **Controller Transceiver**: 2.4GHz ELRS for long distance control
	- **Video Transmitter**(VTX): ...
- **Peripherals**: utility ICs onboard for diagnostics and general drone support
	- **GPS**: provides general location information(waypoint navigation, holding position, and mapping back to origin failsafe)
	- **IMU**: provides angular velocity/rotational data for drone balancing, linear acceleration, and gravitational orientation(provides heading if needed)
	- **Barometer**: maintain drone flight altitude
	- 
# Battery
This is mostly dependent on the four motors as they are the main power draw in the whole apparatus. 
```math
Runtime(hours) = Battery Capacity (Wh) / Device Power Draw(W)
```

# Quad Motors

## Motor

## ESC


# Flight Computer
## Transceiver


# Ground Station
USBC module with ELRS, GPS for local position(allows for return-to-home option), and ... for collecting streamed data from drone(mapping, video, etc). This systems is comprised of three major parts: the plug in transceiver, ground station computer for GUI control and data collection, and the optional control option(external module with joysticks or just using key binds on computers keyboard).
## Hardware



## Software
GUI app contains several main parts: 


# Payload Options
Platform supports a variety of swappable payloads including 
## Lidar 
## 4k Camera
## Thermal Vision



# Features
Drone hosts a variety of features that aren't mandatory to drone operation; however, are noted and may or may not feature a variety of options - to toggle upon your digression. Take into consideration all possible feature as failing to fully understand their function may result in drone failure.
Drone may also be programmable, but please use best programming practices and ensure a variety of testing as any small error can lead to the drone failure.

| Feature          | Options | Design Spec                                                                                                                                                                                                                                                                                                                                                                                                                                  | Implementation                                                                                                                        |     |
| ---------------- | ------- | -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------- | --- |
| Fuel Guage       |         | Provides rough estimate of residual power left in the Li-ion battery. When running low, warns ground-station and at a certain point provides return-to-home or safe-descend option to not power off mid air                                                                                                                                                                                                                                  | **MAX17048**: connected in parallel to the battery to measure battery voltage and provides rough estimates of percentage battery left |     |
| Return to Origin |         | Automatic or manual activation mapping between GPS sources to return to origin. If automatic or chosen by operator, reduces drone utilities for most efficient backtracking(powers off payload, cameras, and non-utilized flight controller modules to focus power for gps, barometer to maintain altitude and hopefully not run into obstacles, flight computer for computational tasks of getting A :arrow_right: B, and the four motors). | Firmware depended utilizing GPS from onboard flight controller and GPS of transceiver                                                 |     |
| Test Points      |         | Please references [Test Points](#test-points) for schematic data after purchasing and plugging boards into power to ensure it properly works                                                                                                                                                                                                                                                                                                 | On PCB boards                                                                                                                         |     |
|                  |         |                                                                                                                                                                                                                                                                                                                                                                                                                                              |                                                                                                                                       |     |

## Test Points
*TODO: should provide all boards and their test point's target voltage*

# Bill of Materials

| Module          | Product Name     | Quantity | Specs       | Price | Link |     |
| --------------- | ---------------- | -------- | ----------- | ----- | ---- | --- |
| ESC             |                  |          |             |       |      |     |
|                 | N-Channel Mosfet | 24       | V~DS~ >= 30 |       |      |     |
|                 |                  |          |             |       |      |     |
|                 |                  |          |             |       |      |     |
|                 |                  |          |             |       |      |     |
| Flight Computer |                  |          |             |       |      |     |
|                 |                  |          |             |       |      |     |
|                 |                  |          |             |       |      |     |
|                 |                  |          |             |       |      |     |
