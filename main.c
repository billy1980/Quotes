// Last updated 2016/02/19 10:00

#include <wand/magick_wand.h>

/*
         0 character width
         1 character height
         2 ascender
         3 descender
         4 text width
         5 text height
         6 maximum horizontal advance
         7 bounding box: x1
         8 bounding box: y1
         9 bounding box: x2
        10 bounding box: y2
        11 origin: x
        12 origin: y

example font metrics from my tcl script test_metrics.tcl

Arial 48
CWxCH = 48.0 x 48.0 AxD = 44.0 x -11.0 TWxTH = 248.0 x 55.0 MHA = 96.0
   BB = (0.0,0.0) (48.0,35.0) origin = 249.0 x 0.0

Arial 24
CWxCH = 24.0 x 24.0 AxD = 22.0 x -6.0 TWxTH = 125.0 x 28.0 MHA = 48.0
   BB = (0.0,0.0) (23.0,17.0) origin = 126.0 x 0.0

Times-New-Roman 18
CWxCH = 18.0 x 18.0 AxD = 17.0 x -4.0 TWxTH = 87.0 x 21.0 MHA = 36.0
   BB = (0.0,0.0) (16.859375,13.0) origin = 87.0 x 0.0

Times-New-Roman 72
CWxCH = 72.0 x 72.0 AxD = 65.0 x -16.0 TWxTH = 352.0 x 83.0 MHA = 144.0
   BB = (0.0,-1.0) (67.453125,50.0) origin = 352.0 x 0.0
*/

/* ----------------------------------------------------------------------------------------------------------------- 
   Set up the drawingwand "dw" for the given font name, font size, and colour.
   If the font or size changes return the new width of a space (metric[4]) in sx
   (the magickwand is required if it is necessary to query the font metrics)
------------------------------------------------------------------------------------------------------------------*/

void draw_setfont(MagickWand *mw,DrawingWand *dw,unsigned char *font,int size,unsigned char *colour,int *sx) {
        double *fm;
        PixelWand *pw = NULL;
        int sflag = 0;

        if(font && *font) {
                DrawSetFont(dw,font);
                sflag = 1;
        }
        if(colour && *colour) {
                pw = NewPixelWand();
                PixelSetColor(pw,colour);
                DrawSetFillColor(dw,pw);
                DestroyPixelWand(pw);
                sflag = 1;
        }
        if(colour && *colour) {
                pw = NewPixelWand();
                PixelSetColor(pw,colour);
                DrawSetFillColor(dw,pw);
                DestroyPixelWand(pw);
                sflag = 1;
        }
        if(size) {
                DrawSetFontSize(dw,size);
        }
        // If either the font or the fontsize (or both) have changed
        // we need to get the size of a space again
        if(sflag) {
                fm = MagickQueryFontMetrics(mw, dw, " ");
                *sx = (int)fm[4];
                RelinquishMagickMemory(fm);
        }
}

/*-------------------------------------------------------------------------------------------------------------------
  sx is the width of a space in the current font and fontsize. 
  If the font or fontsize is changed a new value for the space
  width must be obtained before calling this again (by calling draw_setfont)

  The easiest (?) way to handle vertical text placement is
  not to use gravity at all because then you know that all the text is
  placed relative to ImageMagick's (0,0) coordinate which is the top left of
  the screen and the baseline will be y=0.
-------------------------------------------------------------------------------------------------------------------*/

void draw_metrics(MagickWand *mw,DrawingWand *dw,int *dx,int *dy,int sx,unsigned char *text) {
        double *fm;

        MagickAnnotateImage(mw, dw, *dx, *dy, 0, text);
        MagickDrawImage(mw,dw);
        // get the font metrics
        fm = MagickQueryFontMetrics(mw, dw, text);
        if(fm) {
                // Adjust the new x coordinate
                *dx += (int)fm[4]+sx;
                RelinquishMagickMemory(fm);
        }
}

/*-------------------------------------------------------------------------------------------------------------------
  The main() function
-------------------------------------------------------------------------------------------------------------------*/

void main() {

        MagickWand *mw = NULL;
        DrawingWand *dw = NULL;
        PixelWand *pw = NULL;
        // Current coordinates of text
        int dx,dy;
        int dxa = 450;
        int dxb = 100;
        int dxc = 250;
        int dya = 150;
        int dyb = 180;
        int dyc = 210;
        int dt = 250;
        int dz = 150;
        int dtx = 15;
        int dzx = 15;
        // Width of a space in current font/size  
                int sx;
        double *fm = NULL;

        MagickWandGenesis();
        mw = NewMagickWand();
        dw = NewDrawingWand();
        pw = NewPixelWand();
        // Set the size of the image
        MagickSetSize(mw,851,315);
//      MagickSetImageAlphaChannel(mw,SetAlphaChannel);
        MagickReadImage(mw,"plasma_rnd5.jpg");

        // DO NOT SET GRAVITY - it makes text placement more complicated
        // (unless it does exactly what you wanted anyway).

        // Start near the left edge
        dx = 15;

        // If we know the largest font we're using, we can set the y coordinate
        // fairly accurately. In this case it is the 72 point Times font, so to
        // place the text such that the largest almost touches the top of the image
        // we just add the text height to the descender to give the coordinate for
        // our baseline.
        // In this case the largest is the 72 point Times-New-Roman so I'll use that
        // to compute the baseline
        DrawSetFontSize(dw,72);
        DrawSetFont(dw,"Times-New-Roman");
        fm = MagickQueryFontMetrics(mw, dw,"M");
        dy = fm[1]+fm[4];
        // Note that we must free up the fontmetric array once we're done with it
        RelinquishMagickMemory(fm);


// this
        draw_setfont(mw,dw,"Arial",48,"#ffffff",&sx);
        draw_metrics(mw,dw,&dxa,&dya,sx,"Jeder von uns");

// is
        // A NULL signals to draw_setfont that the font stays the same
        draw_setfont(mw,dw,NULL,24,"#ffffff",&sx);
        draw_metrics(mw,dw,&dxb,&dyb,sx,"ist sein eigener Teufel,");

// my
        draw_setfont(mw,dw,"Times-New-Roman",18,"#ffffff",&sx);
        draw_metrics(mw,dw,&dxc,&dyc,sx,"und wir machen uns diese Welt zur Hölle.");

// text
        draw_setfont(mw,dw,NULL,11,"#0F0FBF",&sx);
        draw_metrics(mw,dw,&dtx,&dt,sx,"O. Wilde (1856 - 1900)");
        MagickDrawImage(mw,dw);

        // Now write the magickwand image
        MagickWriteImage(mw,"O_Wilde_Welt_zur_Hoelle.jpg");

        if(mw)mw = DestroyMagickWand(mw);
        if(dw)dw = DestroyDrawingWand(dw);
        if(pw)pw = DestroyPixelWand(pw);

        MagickWandTerminus();
}
