#!/usr/bin/python3

class Students:

    num_of_students = 0

    def __init__(self, name, year, course):
        self.name = name
        self.year = year
        self.course = course

        Students.num_of_students += 1

    def display(self):
        print(f"{self.name} {self.year} {self.course}")

student1 = Students("Bonie", 2, "Tecnology")
student2 = Students("Ben", 3, "BCOM")

student1.display()
print(Students.num_of_students)
