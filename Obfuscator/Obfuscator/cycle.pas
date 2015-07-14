PROGRAM TestString;

VAR
   f: TEXT;
   st: STRING;
   i: BYTE;
   j, n:INTEGER;
BEGIN
n := 9;
For i:=1 to 12 do
   begin
       j := i * 2;
   end;
{for i:=1 to n-1 do
begin
   j := j * i;
end;}
for i := 1 to 3 do
begin
  j := j * i;
end;
write(j);
END.

