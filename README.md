# Geomteric modelling

University projects using OpenGL

## Subdivision for surfaces

### Winged-Edge data structure

https://pages.mtu.edu/~shene/COURSES/cs3621/NOTES/model/winged-e.html

![tetrahedron](/data/wing-2.jpg)

<TABLE BORDER=3>
     <TR ALIGN=CENTER>
          <TD> <B><I>Edge</I></B> </TD>
          <TD COLSPAN=2> <B><I>Vertices</I></B> </TD>
          <TD COLSPAN=2> <B><I>Faces</I></B> </TD>
          <TD COLSPAN=2> <B><I>Left Traverse</I></B> </TD>
          <TD COLSPAN=2> <B><I>Right Traverse</I></B> </TD>
     </TR>
     <TR ALIGN=CENTER>
          <TD> <B><I>Name</I></B> </TD>
          <TD> <B><I>Start</I></B> </TD>
          <TD> <B><I>End</I></B> </TD>
          <TD> <B><I>Left</I></B> </TD>
          <TD> <B><I>Right</I></B> </TD>
          <TD> <B><I>Pred</I></B> </TD>
          <TD> <B><I>Succ</I></B> </TD>
          <TD> <B><I>Pred</I></B> </TD>
          <TD> <B><I>Succ</I></B> </TD>
     </TR>
     <TR ALIGN=CENTER>
          <TD> a </TD>
          <TD> A </TD>  <TD> D </TD>
          <TD> 3 </TD>  <TD> 1 </TD>
          <TD> e </TD>  <TD> f </TD>
          <TD> b </TD>  <TD> c </TD>
     </TR>
     <TR ALIGN=CENTER>
          <TD> b </TD>
          <TD> A </TD>  <TD> B </TD>
          <TD> 1 </TD>  <TD> 4 </TD>
          <TD> c </TD>  <TD> a </TD>
          <TD> f </TD>  <TD> d </TD>
     </TR>
     <TR ALIGN=CENTER>
          <TD> c </TD>
          <TD> B </TD>  <TD> D </TD>
          <TD> 1 </TD>  <TD> 2 </TD>
          <TD> a </TD>  <TD> b </TD>
          <TD> d </TD>  <TD> e </TD>
     </TR>
     <TR ALIGN=CENTER>
          <TD> d </TD>
          <TD> B </TD>  <TD> C </TD>
          <TD> 2 </TD>  <TD> 4 </TD>
          <TD> e </TD>  <TD> c </TD>
          <TD> b </TD>  <TD> f </TD>
     </TR>
     <TR ALIGN=CENTER>
          <TD> e </TD>
          <TD> C </TD>  <TD> D </TD>
          <TD> 2 </TD>  <TD> 3 </TD>
          <TD> c </TD>  <TD> d </TD>
          <TD> f </TD>  <TD> a </TD>
     </TR>
     <TR ALIGN=CENTER>
          <TD> f </TD>
          <TD> A </TD>  <TD> C </TD>
          <TD> 4 </TD>  <TD> 3 </TD>
          <TD> d </TD>  <TD> b </TD>
          <TD> a </TD>  <TD> e </TD>
     </TR>
</TABLE>

<TABLE BORDER=3>
     <TR ALIGN=CENTER>
          <TD> <B><I>Vertex Name</B></I> </TD>
          <TD> <B><I>Incident Edge</B></I>
     </TR>
     <TR ALIGN=CENTER>
          <TD> A </TD>   <TD> a </TD>
     </TR>
     <TR ALIGN=CENTER>
          <TD> B </TD>   <TD> b </TD>
     </TR>
     <TR ALIGN=CENTER>
          <TD> C </TD>   <TD> d </TD>
     </TR>
     <TR ALIGN=CENTER>
          <TD> D </TD>   <TD> e </TD>
     </TR>
</TABLE>

<TABLE BORDER=3>
     <TR ALIGN=CENTER>
          <TD> <B><I>Face Name</B></I> </TD>
          <TD> <B><I>Incident Edge</B></I>
     </TR>
     <TR ALIGN=CENTER>
          <TD> 1 </TD>   <TD> a </TD>
     </TR>
     <TR ALIGN=CENTER>
          <TD> 2 </TD>   <TD> c </TD>
     </TR>
     <TR ALIGN=CENTER>
          <TD> 3 </TD>   <TD> a </TD>
     </TR>
     <TR ALIGN=CENTER>
          <TD> 4 </TD>   <TD> b </TD>
     </TR>
</TABLE>


### Schemes
http://multires.caltech.edu/pubs/sig00notes.pdf

* Loop scheme
* Modified butterfly scheme


File format:

* obj: recommended
* x3d: successor of vml
* ply
