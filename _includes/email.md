{% assign _email = site.email | split:"@" %}
<style>
  .email:after{ content:'@{{ _email[1] }}'; }
</style>
{% capture safe_email %}
  <span class="email" id="email" onclick="sendMail()">{{ _email[0] }}</span>
{% endcapture %}
<script>
  function sendMail(){
    window.location = 'mailto:' + document.getElementById("email").textContent
      + '@{{ _email[1] }}';
  }
</script>
