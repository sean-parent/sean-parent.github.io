<p style='text-align: center;' markdown='1'>

{% assign figure-number = figure-number | default: 0 | plus: 1 %}
{% assign figure-index = figure-number | minus: 1 %}

{% assign -figure-ref = '[figure ' | append: page.chapter | append: '.' | append: figure-number
        | append: '](#' | append: include.name | append: ')' %}

{% if figure-reference %}
    {% assign figure-reference = ((figure-reference | join: '$') | append: '$'
        | append: -figure-ref) | split: '$' %}
{% else %}
    {% assign figure-reference = -figure-ref | split: '$'  %}
{% endif %}

![figure {{page.chapter}}.{{figure-number}}: {{include.caption}}][{{include.name}}]\\

Figure {{page.chapter}}.{{figure-number}}: {{include.caption}}
</p>{: #{{include.name}} }

[{{include.name}}]: figures/{{include.name}}.svg
