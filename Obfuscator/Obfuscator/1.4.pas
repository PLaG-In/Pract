{1.4. –еликтова€ роща (6)
¬ заповеднике растет роща реликтовых деревьев. ƒл€ их защиты требуетс€ обнести рощу
забором. Ќо дл€ обеспечени€ доступа к остальной территории заповедника площадь участка,
окруженного забором, должна быть минимальной. ƒеревь€ растут точно в узлах координатной
сетки на рассто€нии одного метра друг от друга. Ћюбое из деревьев имеет хот€ бы одного соседа
(с юга, севера, востока или запада). «абор состоит из блоков длиной в один метр.
ѕо заданной конфигурации рощи найти минимально необходимое число блоков дл€ забора.}
Program Forest;

Const
  tree = 1;
  step_label = -1;
  
Var
  Map : Array [1..100,1..100] Of Integer;
  n, m : Integer;

Function Build_Fence : Integer;
Var
  i, j, fence_items_count : Integer;
Begin
  fence_items_count := 0;
  For i := 2 To n - 1 Do
   Begin
    For j := 2 To m - 1 Do
    Begin
      If Map[i,j] = 1 Then
      Begin
        If Map[i,j+1] = -1 Then Inc(fence_items_count);
        If Map[i+1,j] = -1 Then Inc(fence_items_count);
        If Map[i,j-1] = -1 Then Inc(fence_items_count);
        If Map[i-1,j] = -1 Then Inc(fence_items_count);
      End;
    End;
  End;
  Build_Fence := fence_items_count;
End;

Procedure Check_Around_Recurs(x, y : Integer);
Begin
  If (x in [1..n]) And (y in [1..m]) Then
  Begin
    If (y < m) Then
    Begin
      If (Map[x,y+1] = 0) Then
      Begin
        Map[x,y+1] := step_label;
        Check_Around_Recurs(x,y + 1);
      End;
    End;
    If (x < n) Then
    Begin
      If (Map[x+1,y] = 0) Then
      Begin
        Map[x+1,y] := step_label;
        Check_Around_Recurs(x + 1,y);
      End;
    End;
    If (x > 1) Then
    Begin
      If (Map[x-1,y] = 0) Then
      Begin
        Map[x-1,y] := step_label;
        Check_Around_Recurs(x - 1,y);
      End;
    End;
    If (y > 1) Then
    Begin
      If (Map[x,y-1] = 0) Then
      Begin
        Map[x,y-1] := step_label;
        Check_Around_Recurs(x,y - 1);
      End;
    End;
  End;
End;

Procedure Fill_Map(k, p : Integer);
Var
  i, j : Integer;
Begin
  For i := 1 To k Do
  Begin
    For j := 1 To p Do
    Begin
      Map[i,j] := 0;
    End;
  End;
End;

Procedure Read_Map;
Var
  i, j, k, prom : Integer;
  temp : String;
  A : Text;
Begin
  k := 1;
  Assign(A, 'input.txt');
  Reset(A);
  Read(A, n);
  Readln(A, m);
  Inc(n);Inc(n);
  Inc(m);Inc(m);
  Fill_Map(n,m);
  While not EOF(A) Do
  Begin
    For i := 2 to n - 1 Do
    Begin
      Readln(A, temp);
      For j := 2 to m - 1 Do
      Begin
        prom := Ord(temp[k]) - 48;
        Map[i,j] := prom;
        Inc(k);
      End;
    k := 1;
    End;
  End;
  Close(A);
End;

Procedure Write_Out(fence_count : Integer);
Var
  text_output : Text;
Begin
  Assign(text_output, 'output.txt');
  Rewrite(text_output);
  Write(text_output, fence_count);
  Close(text_output);
End;

Begin
  Read_Map;
  Map[1,1] := step_label;
  Check_Around_Recurs(1,1);
  Write_Out(Build_Fence);
End.