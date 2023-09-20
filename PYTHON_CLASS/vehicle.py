#!/usr/bin/python3

class vehicle:

    def __init__(self, max_speed, mileage):

        self.max_speed = max_speed
        self.mileage = mileage

vehicle_specs = vehicle(260, 20000)
print(vehicle_specs.max_speed, vehicle_specs.mileage)
