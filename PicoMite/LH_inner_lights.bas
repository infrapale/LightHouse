'Lighthouse Indoor Lights
Option explicit
Dim b%(4) = (RGB(127,100,10),RGB(GREEN),RGB(BLUE),RGB(YELLOW),RGB(CYAN))
SetPin GP8, DOUT
Const NBR_PIX= 13
Dim integer r_g_b(NBR_PIX)
Dim cdot(3) As integer


Bitbang WS2812 O,GP8,5,b%()

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

Const DARK_IF_LESS  = 1.0
Const MEAS_EVERY_N  = 2
Dim integer led(LED_NBR-1) = (LED_BLUE1, LED_WHITE1, LED_RED1, LED_GREEN1,LED_YELLOW1,LED_RED2,LED_WHITE2,LED_YELLOW2)
SetPin GP26, AIN

Dim i As integer
For i = 0 To LED_NBR-1
  SetPin led(i), DOUT
  Pin(led(i)) = 0
Next i

Dim meas_iterations As integer
Dim show_iterations As integer
Dim is_dark As integer
Dim led_indx As integer
Dim led_d As integer
Dim keep_dark As integer
led_indx = 0: led_d = 1


Sub wheel Position
  Select Case Position
    Case IS < 0, IS > 255
      cdot(0) =0: c(1)=0: c(2)=0
    Case is < 85
      cdot(0) = Position * 3
      cdot(1) = 255 -Position * 3
      cdot(2) = 0
    Case is < 170
      Position = Position - 85
      cdot(0) = 255 -Position * 3
      cdot(1) = 0
      cdot(2) = Position * 3
    Case Else
      Position = Position -170
      cdot(0) = 0
      cdot(1) = Position * 3
      cdot(2) = 255 -Position * 3

  End Select
End Sub


Sub rgb_show
  Bitbang WS2812 O,GP14,NBR_PIX,r_g_b()
End Sub


Sub kit_lights
  Pin(led(led_indx)) = 0
  led_indx = led_indx + led_d
  If led_indx < 0 Then
    led_indx = 1: led_d = 1
  ElseIf led_indx > LED_NBR - 1 Then
    led_indx = LED_NBR -1: led_d = -1
  EndIf
  If Not(is_dark Or keep_dark) Then
    Pin(led(led_indx)) = 1
  EndIf
End Sub

Sub light_show
  Local Dim integer i,j,k,n,indx
  For n = 0 To 255
    For j = 0 To 255
      For i = 0 To NBR_PIX
        indx = (i * 256 / NBR_PIX)+j
        wheel(indx And 255)
        If is_dark Or keep_dark Then
          r_g_b(i) =RGB(0,0,0)
        Else
          r_g_b(i) =(RGB(cdot(0),cdot(1),cdot(2)))
        EndIf
        rgb_show()
        'Print n,j,i,k,cdot(k)
      Next i
    Next j
    kit_lights()
    Print Pin(GP26)
  Next n
End Sub


show_iterations = 0
While 1 do
  show_iterations = show_iterations + 1
  If show_iterations < MEAS_EVERY_N Then
    keep_dark = 0
  Else
    keep_dark = 1
    show_iterations = 0
  EndIf
  light_show()
  If keep_dark Then
    If Pin(GP26) < DARK_IF_LESS Then
      is_dark = 1
    EndIf
  EndIf

Loop                                                                                          