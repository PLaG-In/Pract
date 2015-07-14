PROGRAM TestString;

VAR
   f: TEXT;
   st: STRING;
   i: BYTE;
BEGIN
     ASSIGN(f, 'inputto.txt');
     RESET(f);
     READLN(f, st);
     i := 1;
     WHILE (st[1]=' ') and (LENGTH(st) > 0)
     DO
      DELETE(st,1,1);
     REPEAT
        IF COPY(st,i,2) = '  '
        THEN
            st := COPY(st, 1, i) + COPY(st, i + 2, LENGTH(st))
        ELSE
            i := i + 1
     UNTIL (i > LENGTH(st));
     IF COPY(st, i - 1, 1 ) = ' '
     THEN
         st := COPY(st, 1, i - 2);
     CLOSE(f);
     ASSIGN(f, 'out.txt');
     REWRITE(f);
     WRITE(f, st);
     CLOSE(f);
END.

