' GPIO Test
Option EXPLICIT

Option LIST
Print MM.HRes, MM.VRes
Print MM.Info(FONTHEIGHT), MM.Info(FONTWIDTH)

Dim h_midpoint As integer
Dim v_midpoint As integer
h_midpoint = MM.HRes/2: v_midpoint = MM.VRes/2
CLS RGB(BLUE)
Line 0,0,MM.HRes-1,MM.VRes-1,,RGB(yellow)

Dim i As integer
For i= 0 To 100 Step 5
  Pixel h_midpoint+i, v_midpoint+i,RGB(white)
Next i

Dim txt As string
txt = "Mikrofan"
Print txt
Box 10,10,120,100,3,RGB(red),RGB(black)
RBox 100,100,120,100,10,RGB(red),RGB(green)
Circle h_midpoint,v_midpoint,50,4,1,RGB(white),RGB(cyan)
Text 20,100,txt,"N",5                                                