

{% assign code-number = code-number | default: 0 | plus: 1 %}
{% assign code-index = code-number | minus: 1 %}

{% assign -code-ref = '[code ' | append: page.chapter | append: '.' | append: code-number
        | append: '](#' | append: include.name | append: ')' %}

{% if code-reference %}
    {% assign code-reference = ((code-reference | join: '$') | append: '$'
        | append: -code-ref) | split: '$' %}
{% else %}
    {% assign code-reference = -code-ref | split: '$'  %}
{% endif %}

```cpp
{% include_relative code/{{include.name}}.cpp %}
```
<p style='text-align: center;' markdown='1'>
Code {{page.chapter}}.{{code-number}}: {{include.caption}}
</p>{: #{{include.name}} }
