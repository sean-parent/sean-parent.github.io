---
title: Better Code
tagline: Goals for Software Developers
layout: page
tags: [ better-code ]
---

## Contents

<dl class='posts'>
{% for p in site.pages %}
    {% if p.tags contains 'better-code' and p.url != page.url %}
        <dt><a href="{{ BASE_PATH }}{{ p.url }}">{{ p.title }}</a></dt>
            {% if p.tagline %} <dd><span>Goal: {{ p.tagline }}</span></dd> {% endif %}
    {% endif %}
{% endfor %}
</dl>
