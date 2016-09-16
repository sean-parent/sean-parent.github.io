<p style='text-align: center;' markdown='1'>

{% assign figure-number = figure-number | default: 0 | plus: 1 %}
{% assign figure-index = figure-number | minus: 1 %}

{% assign -figure-ref = '[figure ' | append: page.chapter | append: '.' | append: figure-number
        | append: '](#' | append: figure-name | append: ')' %}

{% if figure-reference %}
    {% assign figure-reference = ((figure-reference | join: '$') | append: '$'
        | append: -figure-ref) | split: '$' %}
{% else %}
    {% assign figure-reference = -figure-ref | split: '$'  %}
{% endif %}

![figure {{page.chapter}}.{{figure-number}}: {{figure-caption}}][{{figure-name}}]\\

figure {{page.chapter}}.{{figure-number}}: {{figure-caption}}
</p>{: #{{figure-name}} }

[{{figure-name}}]: figures/{{figure-name}}.svg
