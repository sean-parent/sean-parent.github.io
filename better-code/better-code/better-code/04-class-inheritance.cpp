//
//  main.cpp
//  better-code
//
//  Created by Sean Parent on 10/3/16.
//  Copyright © 2016 Sean Parent. All rights reserved.
//

#include <iostream>
#include <memory>
#include <vector>

using namespace std;

// <include>
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

int test_03();

int main() {
    test_03();
    document doc;
    doc.emplace_back(make_shared<circle>(42.0));
    draw(doc, cout, 0);
}
