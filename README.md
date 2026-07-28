<div align="center">

  <h1>⚡ OOP Final Project — METAL SLUG</h1>
  <p><b>Designed & Engineered for CS Semester 2</b></p>

  [![Language](https://img.shields.io/badge/Language-C%2B%2B17-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)](#)
  [![Build](https://img.shields.io/badge/Compiler-GCC%20%2F%20Clang-brightgreen?style=for-the-badge)](#)
  [![Course](https://img.shields.io/badge/Course-Object%20Oriented%20Programming-blueviolet?style=for-the-badge)](#)

</div>

---

### 💬 Behind the Project

This repo holds our final submission for the Object-Oriented Programming (CS 2nd Semester) course. 

Honestly, this took a lot out of us. Between late-night debugging sessions, fighting memory leaks, and refactoring class structures when things got messy, **Ayaan** and I spent weeks turning initial whiteboard sketches into a fully functional C++ system. We didn't just want to meet the rubric—we wanted to build something fast, modular, and clean.

---

### 🛠️ C++ & OOP Architecture

We built everything natively in pure C++, relying on standard library features (`std::vector`, smart pointers, RAII) while sticking strictly to solid OOP fundamentals.

<details>
<summary>🔑 <b>Click to expand how we implemented core OOP pillars</b></summary>

<br>

* **Encapsulation:** All critical member variables are strictly private/protected. State modification only happens through validated member functions to prevent dangling references or illegal operations.
* **Inheritance:** Built extensible base classes so specialized components inherit core behaviors without code duplication.
* **Polymorphism:** Leveraged dynamic dispatch via `virtual` methods, pure virtual functions (interfaces), and runtime method overriding.
* **Memory Management:** Used RAII principles to ensure resources clean up after themselves, avoiding heap memory leaks.

</details>

---

### 📐 System Design & UMLs

Before writing a single line of code, we mapped out the system logic using UML diagrams:

* **Use Case Diagrams:** To nail down user interactions and permissions.
* **Class Diagrams:** To visualize inheritance chains, compositions, and relationships between objects.
