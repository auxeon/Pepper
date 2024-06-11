#include <pepper.h>


void draw_polygon(GLenum mode, ps_vec2 point, double delta, double angle, double radius, ps_color color, bool fliph){
    glBegin(mode);
    double alpha = ps_clamp(color.a,0.0f,1.0f);
    glColor3f((GLfloat)sin(color.r) * (GLfloat)alpha, (GLfloat)sin(color.g) * (GLfloat)alpha, (GLfloat)sin(color.b) * (GLfloat)alpha);
    const double dx = 0.0f;
    const double dy = 0.0f;
    const double lo = fliph?-angle/2.0:0.0;
    const double hi = fliph?angle/2.0:angle;
    for(double i=lo;i<hi;i+=delta){
        glVertex2f((GLfloat)point.x + (GLfloat)radius * (GLfloat)cos(ps_deg2rad(i)), (GLfloat)point.y + (GLfloat)radius * (GLfloat)sin(ps_deg2rad(i)));
    }
    glEnd();
}

void run(){
    if (FPS){
        PS_INFO("[%s] : opengl windowing + %dFPS",__FUNCTION__,FPS);
    }
    else{
        PS_INFO("[%s] : FPS set to 0",__FUNCTION__);
        PS_INFO("[%s] : opengl windowing + MAX FPS",__FUNCTION__);
    }

    ps_clock_data* t = ps_clock_get();
    ps_clock_data* c = ps_clock_get();
    ps_clock_start(t);
    ps_clock_start(c);
    
    ps_window* window = ps_window_get_handle();
    ps_window_init(window,APPNAME,APPW,APPH);
    bool is_running = true;

    ps_color color1 = (ps_color){PS_COLOR_EMERALD, .a=1.0};
    ps_color color2 = (ps_color){PS_COLOR_RED, .a=1.0};
    bool isgreen = false;
    double shapes_delta[] = {120.0,90.0,60.0,45.0,30.0,15.0,5.0};
    double shapes_time[] = {1.0,1.0,1.0,1.0,1.0,1.0,1.0};
    int total_shapes = sizeof(shapes_delta)/sizeof(shapes_delta[0]);

    int mode = 0;
    double delta = shapes_delta[mode];
    double duration = shapes_time[mode];
    char buffer[WINDOW_TITLE_LEN];
    PS_INFO("%s opengl vendor\n",(char*)glGetString(GL_VENDOR));
    ps_vec2 pos = (ps_vec2){
        .x=0.0,
        .y=0.0
    };
    ps_color colorz = isgreen?color1:color2;
    while(is_running){
        ps_window_poll_events(window);
        ps_window_input_update(window);
        if(ps_window_input_keyboard_istriggered_down(window, PS_KEY_SPACE) || ps_window_input_mouse_istriggered_down(window, PS_MOUSE_BUTTON_1)) {
            isgreen = !isgreen;
            PS_INFO("space pressed %d",isgreen);
            PS_INFO("%f %f %f ",colorz.r,colorz.g,colorz.b);
        }
        colorz = isgreen?color1:color2;
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        ps_vec2 mouse_position = ps_window_input_mouse_get_position(window);
        ps_vec2 window_size = ps_window_get_size(window);
        mouse_position.x = (mouse_position.x / window_size.x)*2.0 - 1.0;
        mouse_position.y = (1.0 - (mouse_position.y/window_size.y))*2.0 - 1.0;
        for (int i = 1; i < 10; ++i) {
            pos = (ps_vec2){
                .x=mouse_position.x + cos(ps_clock_uptime(t)*((double)i+1)*0.5 + 10*(double)i*PI/180.0f)*0.2,
                .y=mouse_position.y + sin(ps_clock_uptime(t)*((double)i+1)*0.5 + 10*(double)i*PI/180.0f)*0.2
            };

            draw_polygon(
                GL_LINE_LOOP,
                pos,
                shapes_delta[mode],
                360.0, 
                0.05, 
                colorz,
                false
            );

         }

        // print_ps_vec2(mouse_position);
        ps_clock_update(c,FPS);
        sprintf(buffer, "[%s] [%fx%f] (%0.3lf FPS)", APPNAME, window_size.x, window_size.y, ps_clock_fps(c));
        ps_window_set_title(window, buffer);
        ps_clock_reset(c);
        if(ps_clock_uptime(c) > shapes_time[mode]){
            mode = (mode+1)%(total_shapes);
            ps_clock_reset_uptime(c);
        }
        ps_window_swap_buffers(window);
        is_running = (!ps_window_should_close(window) || ps_window_input_keyboard_istriggered_down(window, PS_KEY_Q));
        if (ps_window_input_keyboard_istriggered_down(window, PS_KEY_Q)) {
          PS_INFO("quit\n");
        }
    }
    ps_window_destroy(window);
    ps_window_release(window);
    ps_clock_stop(c);
    ps_clock_stop(t);
    PS_INFO("total time : %lfs",ps_clock_uptime(t));
    // (void)getchar();
}


int main() {
  run();
}
