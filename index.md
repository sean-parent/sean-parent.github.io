---
layout: page
title: "Sean Parent"
tagline: "Developer Notes"
---
{% include JB/setup %}

<ul class="entries">
  {% for post in site.posts %}
	<a href="{{ post.url }}">
		<h3>{{ post.title }}</h3>
	</a>
	{{ post.content | replace:'more start -->','' | replace:'<!-- more end','' }}
  {% endfor %}
</ul>
