//Shape.h
#pragma once

class Vector; // избежал цикл. include

class Shape {
public:
    Shape();
    virtual ~Shape(); 

    virtual void Move(const Vector& v) = 0;
    virtual void Out() const  = 0;
    virtual double Area() const  = 0;

    static void PrintCount();
    static void PrintShapes();

    static int GetCount() { return Count; }; 
    static Shape* GetHead() { return head_; }
    
    Shape* GetNext() const { return next_shape; }
private:
    static int Count; 
    
    Shape* next_shape;
    static Shape* head_;
    static Shape* tail_;
};