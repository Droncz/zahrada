<html>
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width; initial-scale=1; shrink-to-fit=yes; maximum-scale=2.0;">
<link rel="stylesheet" type="text/css" href="style.css">
<title>Řízení GPIO</title>
</head>

<body>
<div class="main">
<h1>Switch</h1>
<table>
  <tr>
    <th>Relé</th>
    <th>GPIO</th>
    <th>Nyní</th> 
    <th>Přepnout</th> 
    <th>Povoleno/Zakázáno</th>
  </tr>
  <tr>
    <td>RELAY_1_0</td>
    <td>%RELAY_1_0-gpio%</td>
    <td>%RELAY_1_0-state%</td>
    <td>
      <form>
        <button type="submit" name="RELAY_1_0-state" class="btn" value="%RELAY_1_0-newstate%">%RELAY_1_0-newstate%</button>
      </form>
    </td>
    <td>
      <form>
        <input id="RELAY_1_0-on" class="toggle toggle-left" name="RELAY_1_0-enabled" value="1" type="radio" onchange='this.form.submit();' %RELAY_1_0-checkedon%>
        <label for="RELAY_1_0-on" class="btn">ZAP</label>
        <input id="RELAY_1_0-off" class="toggle toggle-right" name="RELAY_1_0-enabled" value="0" type="radio" onchange='this.form.submit();' %RELAY_1_0-checkedoff%>
        <label for="RELAY_1_0-off" class="btn">VYP</label>
      </form>
    </td>
  </tr>
  <tr>
    <td>RELAY_1_1</td>
    <td>%RELAY_1_1-gpio%</td>
    <td>%RELAY_1_1-state%</td>
    <td>
      <form>
        <button type="submit" name="RELAY_1_1-state" class="btn" value="%RELAY_1_1-newstate%">%RELAY_1_1-newstate%</button>
      </form>
    </td>
    <td>
      <form>
        <input id="RELAY_1_1-on" class="toggle toggle-left" name="RELAY_1_1-enabled" value="1" type="radio" onchange='this.form.submit();' %RELAY_1_1-checkedon%>
        <label for="RELAY_1_1-on" class="btn">ZAP</label>
        <input id="RELAY_1_1-off" class="toggle toggle-right" name="RELAY_1_1-enabled" value="0" type="radio" onchange='this.form.submit();' %RELAY_1_1-checkedoff%>
        <label for="RELAY_1_1-off" class="btn">VYP</label>
      </form>
    </td>
  </tr>
</table>
<p>
<a href="/">HOME</a>
</div>
</body>
</html>