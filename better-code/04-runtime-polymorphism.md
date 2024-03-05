---
title: Runtime Polymorphism
tagline: No Public Inheritance
layout: book-page
tags: [ better-code ]
chapter: 4
---

Object-oriented programming has been one of the paradigms supported by C++ from its invention. The idea of type inheritance and virtual functions were borrowed from Simula[^cpp-history]. Inheritance can represent a subtype or protocol relationship. Although the two are closely related, in this chapter we're primarily concerned with subtype relationships through class inheritance. Protocols are discussed in the next chapter. {::comment}link{:/comment}

With a subtype, or _is-a_, relationship a subclass inherits from a baseclass. The baseclass defines a set of virtual member functions for which the implementation can be overriden by the subclass. The canonical example is a drawing library for shapes:

```cpp
// library.hpp

class shape {
  public:
    virtual ~shape() { }
    virtual void draw(ostream&, size_t) const = 0;
};

using document = vector<shared_ptr<shape>>;

void draw(const document& x, ostream& out, size_t position)
{
    out << string(position, ' ') << "<document>" << endl;
    for (const auto& e: x) e->draw(out, position + 2);
    out << string(position, ' ') << "</document>" << endl;
}

```

```cpp
// main.cpp

class circle final : public shape {
    double _radius;
public:
    explicit circle(double radius) :
        _radius(radius)
        { }
    void draw(ostream& out, size_t position) const override {
        out << string(position, ' ');
        out << "circle: " << _radius << endl;
    }
};

class rectangle final : public shape {
    double _width;
    double _height;
public:
    explicit rectangle(double width, double height) :
        _width(width), _height(height)
        { }
    void draw(ostream& out, size_t position) const override {
        out << string(position, ' ');
        out << "rectangle: " << _width << ", " << _height << endl;
    }
};

int main() {
    document doc;
    doc.emplace_back(make_shared<circle>(42.0));
    draw(doc, cout, 0);
}
```

  {::comment}vtable implementation?{:/comment}



[^cpp-history]:
    Bjarne Stroustrup. "A History of C++: 1979-1991." _History of Programming Language Two_ (Addison-Wesley, 1995). <http://www.stroustrup.com/hopl2.pdf>.
