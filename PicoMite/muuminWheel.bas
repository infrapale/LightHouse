'Wheel WS2812 Example
Option explicit
Dim b%(4) = (RGB(127,100,10),RGB(GREEN),RGB(BLUE),RGB(YELLOW),RGB(CYAN))
SetPin GP8, DOUT
Const NBR_PIX= 13
Dim integer r_g_b(NBR_PIX)
Dim cdot(3) As integer

Bitbang WS2812 O,GP8,5,b%()

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

Sub rainbow
  Dim integer i,j,k,n,indx
  For n = 0 To 255
    For j = 0 To 255
      For i = 0 To NBR_PIX

        indx = (i * 256 / NBR_PIX)+j
        wheel(indx And 255)
        r_g_b(i) =(RGB(cdot(0),cdot(1),cdot(2)))
        rgb_show()
        'Print n,j,i,k,cdot(k)
      Next i
    Next j
  Next n
End Sub

Sub rgb_show
  Bitbang WS2812 O,GP8,NBR_PIX,r_g_b()
End Sub

rainbow()

Dim integer i
For i = 0 To 255
  b%(0) = RGB(i,0,0)
  Bitbang WS2812 O,GP8,5,b%()
Next i                                             