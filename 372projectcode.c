#include <stdio.h>
#include <gtk/gtk.h>

int width = 1000;
int height = 1000;
int running = 1;
/*static cairo_surface_t* surface = NULL;

static void clear_surface(void) {
    cairo_t *cr;
    cr = cairo_create(surface);

    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_paint(cr);

    cairo_destroy(cr);
}

static void resize_cb(GtkWidget *widget, width, height, gpointer data) {
    if (surface) {
        cairo_surface_destroy(surface);
        surface = NULL;
    }
    if (gtk_native_get_surface(gtk_widget_get_native(widget))) {
        surface = gdk_surface_create_similar_surface(gtk_native_get_surface(gtk_widget_get_native(widget)), CAIRO_CONTENT_COLOR, gtk_widget_get_width(widget), gtk_widget_get_height(widget));
        clear_surface();
    }
}

static void draw_cb(GtkDrawingArea *drawing_area, cairo_t *cr, width, height, gpointer data) {
    cairo_set_source_surface(cr, surface, 0, 0);
    cairo_paint(cr);
}

static void draw_particle(GtkWidget* widget, double x, double y) {
    cairo_t *cr;
    cr = cairo_create(surface);

    //dont know if we need cairo_scale
    //cairo_scale(cr, 1, 0.7);
    cairo_arc(cr, x, y, 5, 0, 2 * M_PI);

    cairo_fill(cr);

    cairo_destroy(cr);

    gtk_widget_queue_draw(widget);
}

static void close_window(void) {
    if (surface) {
        cairo_surface_destroy(surface);
    }
}*/

struct Particle {
    int posx;
    int posy;
    int velx;
    int vely;
};

int main(){
  
    //5 is just an arbitrary choice here; we can do however many is necessary for a good test 
    int numParticles = 5;
    struct Particle array[numParticles];
    //initializing the particles; the intial position and velocity is also kind of arbitrary 
    for(int i = 0;i<numParticles;i++) {
        array[i].posx = i;
        array[i].posy = i;
        array[i].velx = 1 + (i * 2);
        array[i].vely = i - 3;
    }

    //end condition will be a stop button in the gui 
    while (running == 1) {
        //i think this for loop is the part we want to parallelize
        for (int j = 0; j < numParticles; j++) {
            //do we want to check for particle collisions? i feel like that'll be really hard to do in parallel specifically 
            int destx = array[j].posx + array[j].velx;
            int desty = array[j].posy + array[j].vely;
            if (destx > width) {
                int remainingdistx = array[j].velx - (width - array[j].posx);
                destx = width - remainingdistx;
                array[j].velx = array[j].velx * -1;
            }
            if (desty > height) {
                int remainingdisty = array[j].vely - (height - array[j].posy);
                desty = height - remainingdisty;
                array[j].vely = array[j].vely * -1;
            }
            array[j].posx = destx;
            array[j].posy = desty;
        }
    }

}
