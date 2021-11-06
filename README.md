# Geomteric modelling

University projects using OpenGL

## Subdivision for surfaces

### Winged-Edge data structure

https://pages.mtu.edu/~shene/COURSES/cs3621/NOTES/model/winged-e.html

![tetrahedron](/data/wing-2.jpg)

Main data table:

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

Other tables:

<TABLE CELLPADDING=4 BORDER=0 >
<TR>
<TD>
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
</TD>
<TD>
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
</TD>
</TR>
</TABLE>


### Schemes
http://multires.caltech.edu/pubs/sig00notes.pdf

In this project the Loop and the modified butterfly schemes will be implemented. 

#### **Loop scheme**
The Loop scheme is a approximating scheme. 
After subdivision the vertices will be moved to an approximated position, so the surface will be smoother.

![Loop scheme](/data/loop-scheme.png)

#### **Modified butterfly scheme**

The modified butterfly is an interpolating scheme, so no vertex will be moved after they were generated.

Each will be split into two, the split point will be calculated according to the scheme:

![Modified butterfly scheme](/data/modified-butterfly-scheme.png)

The calculated vertices will be saved to the old ones.

After that generate new faces using the old and new vertices. From 1 face will be generated 4 new faces.
3 using 1 old vertex and 2 new vertices and 1 face with the new vertices. 
After the generation the old faces will be deleted.

![Modified butterfly scheme new faces](/data/butterfly_new_faces.png)

### File formats:

* obj: recommended
* x3d: successor of vml
* ply
