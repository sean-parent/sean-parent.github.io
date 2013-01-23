---
layout: page
title: "Sean Parent"
tagline: "Developer Notes"
---
{% include JB/setup %}

{% for post in site.posts %}

### [ {{ post.title }} ]( {{ post.url }} )
{{ post.content | replace:'more start -->','' | replace:'<!-- more end','' }}
[more...]( {{ post.url }} )

{% endfor %}
