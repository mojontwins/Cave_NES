' Let's calculate some nice trajectories

Dim As Short y
Dim As Integer yi, yi0
Dim As Integer x
Dim As Short vy
Dim As Double Pi

Const a As Double = 128/(95*95)

'  U; f(0) = 0; f (96) = 128; f(x) = a*x^2; 
' 128 = a*96^2'

'screen 13
Pi = Atn(1)*4

'line (0,192)-(192,192)
'line (96,0)-(96,192)
'line (192,0)-(192,192)
'line (0,128)-(192,128)

yi0 = 128
For x = 0 To 95 Step 2
	'Pset (x, a*(x-95)^2)
	'pset (x, 128-128*Sin(x*(pi/2)/96)),2
	y = a*(x-95)^2
	yi = Int (y)
	Print -(yi - yi0);",";
	yi0 = yi
next x

'sleep
