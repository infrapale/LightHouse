'Wheel 8xLED Test
Option explicit
Const LED_NBR    = 8
Const LED_BLUE1  = 11
Const LED_WHITE1 = 10
Const LED_RED1   = 9
Const LED_GREEN1 = 7
Const LED_YELLOW1= 6
Const LED_RED2   = 5
Const LED_WHITE2 = 4
Const LED_YELLOW2= 2
Dim integer led(LED_NBR-1) = (LED_BLUE1, LED_WHITE1, LED_RED1, LED_GREEN1,LED_YELLOW1,LED_RED2,LED_WHITE2,LED_YELLOW2)

Dim i As integer
For i = 0 To LED_NBR-1
  SetPin led(i), DOUT
  Pin(led(i)) = 0
Next i

Dim led_indx As integer
Dim led_d As integer
led_indx = 0: led_d = 1

Do While 1
  Pin(led(led_indx)) = 0
  led_indx = led_indx + led_d
  If led_indx < 0 Then
    led_indx = 1: led_d = 1
  ElseIf led_indx > LED_NBR - 1 Then
    led_indx = LED_NBR -1: led_d = -1
  EndIf
  Pin(led(led_indx)) = 1
  Pause 100
Loop



Dim n As integer
For n = 0 To LED_NBR-1
  For i = 0 To LED_NBR-1
    If i = n Then
      Pin(led(i)) = 1
    Else
      Pin(led(i)) = 0
    EndIf
  Next i
  Pause 2000
Next n                                                       