# web_server
Requests are handled in src/web_server/session/handle_request/handle_request.hpp

# ApacheBench localhost results
run with: ./ab -n 20000 -c 1000 -w http://localhost/  
behind a apache reverse proxy

<table>

<tbody>

<tr>

<th colspan="2" bgcolor="white">Server Software:</th>

<td colspan="2" bgcolor="white">Boost.Beast/306</td>

</tr>

<tr>

<th colspan="2" bgcolor="white">Server Hostname:</th>

<td colspan="2" bgcolor="white">localhost</td>

</tr>

<tr>

<th colspan="2" bgcolor="white">Server Port:</th>

<td colspan="2" bgcolor="white">80</td>

</tr>

<tr>

<th colspan="2" bgcolor="white">Document Path:</th>

<td colspan="2" bgcolor="white">/</td>

</tr>

<tr>

<th colspan="2" bgcolor="white">Document Length:</th>

<td colspan="2" bgcolor="white">21654 bytes</td>

</tr>

<tr>

<th colspan="2" bgcolor="white">Concurrency Level:</th>

<td colspan="2" bgcolor="white">1000</td>

</tr>

<tr>

<th colspan="2" bgcolor="white">Time taken for tests:</th>

<td colspan="2" bgcolor="white">264.890 seconds</td>

</tr>

<tr>

<th colspan="2" bgcolor="white">Complete requests:</th>

<td colspan="2" bgcolor="white">20000</td>

</tr>

<tr>

<th colspan="2" bgcolor="white">Failed requests:</th>

<td colspan="2" bgcolor="white">108</td>

</tr>

<tr>

<td colspan="4" bgcolor="white">(Connect: 0, Length: 108, Exceptions: 0)</td>

</tr>

<tr>

<th colspan="2" bgcolor="white">Non-2xx responses:</th>

<td colspan="2" bgcolor="white">2</td>

</tr>

<tr>

<th colspan="2" bgcolor="white">Total transferred:</th>

<td colspan="2" bgcolor="white">435551680 bytes</td>

</tr>

<tr>

<th colspan="2" bgcolor="white">HTML transferred:</th>

<td colspan="2" bgcolor="white">431671656 bytes</td>

</tr>

<tr>

<th colspan="2" bgcolor="white">Requests per second:</th>

<td colspan="2" bgcolor="white">75.50</td>

</tr>

<tr>

<th colspan="2" bgcolor="white">Transfer rate:</th>

<td colspan="2" bgcolor="white">1605.74 kb/s received</td>

</tr>

<tr>

<th bgcolor="white" colspan="4">Connection Times (ms)</th>

</tr>

<tr>

<th bgcolor="white"> </th>

<th bgcolor="white">min</th>

<th bgcolor="white">avg</th>

<th bgcolor="white">max</th>

</tr>

<tr>

<th bgcolor="white">Connect:</th>

<td bgcolor="white">0</td>

<td bgcolor="white">13</td>

<td bgcolor="white">2045</td>

</tr>

<tr>

<th bgcolor="white">Processing:</th>

<td bgcolor="white">37</td>

<td bgcolor="white">13051</td>

<td bgcolor="white">13839</td>

</tr>

<tr>

<th bgcolor="white">Total:</th>

<td bgcolor="white">37</td>

<td bgcolor="white">13064</td>

<td bgcolor="white">15884</td>

</tr>

</tbody>

</table>
