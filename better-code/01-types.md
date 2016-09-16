---
title: Types
tagline: No Incomplete Types

layout: book-page
tags: [ better-code ]
chapter: 1
---
{::comment}This section is referenced by chapter[03]{:/comment}

### Definition of _incomplete type_

A _type_ is defined as:

> A type is a pattern for storing and modifying objects.

And an _object_ is defined as:

> An object is a representation of an entity as a value in memory. {::comment}Add reference to section 1.3 of Elements of Programming{:/comment}

The fact that an object exists _in memory_ is important. Objects are physical entities and as such are governed by the laws of physics, despite being oft referred to as _virtual_.

The basic building block for modern computers is the transistor. A transistor is a solid-state electronic switch. Prior to the use of transistors (the first transistor based computer was built in 1953{::comment}Citation{:/comment}) computers were built with vacuum tubes, relays, or gears. {::comment}citation{:/comment} Any device that can serve as a controlled switch can be used to build a computer.

{% assign figure-name= 'transistor' %}
{% assign figure-caption= 'Transistor' %}
{% include figure.md %}
{% assign figure-transistor = figure-index %}

The symbol for a simple transistor is shown in {{figure-reference[figure-index]}}. Voltage applied to the base allows current to flow from the collector to the emitter.

{% assign figure-name='relay' %}
{% assign figure-caption='Relay' %}
{% include figure.md %}
{% assign figure-relay = figure-number %}

{::comment}Discussion of relationships here? or in chapter[03]?{:/comment}



### Why incomplete types are problematic

### How to avoid incomplete types

[transistor]: figures/transistor.svg
