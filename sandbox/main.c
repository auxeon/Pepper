/*
@author Abhikalp Unakal
@date   08 june 2021
@file   main.c
@desc   sandbox launcher and tests
*/

#include "../pch.h"

#ifdef _WIN64
#define random rand
#endif

/*
    setting up stuff needed for test0
*/
int gi_count = 0;

typedef struct node{
    int val;
    int id;
}node;

node node_create(int val, int id){
    node n = {.val=val, .id=id};
    return n;
}
/*
    essential to setup the struct needed for vector ops using macros
*/
ps_vector_declare(node);

void display_macro_version(ps_vector_node* v){
    for(int i=0;i<v->size;++i){
        printf("%d[%d] ",ps_vector_at(*v,i).id, ps_vector_at(*v,i).val);
    }
    printf("\n");
}

void display_merge(void* n, size_t size){
    for(int i=0;i<size;++i){
        printf("%d ",((node*)n)[i].val);
    }
    printf("\n");
}
/*
    has the drawback of not being very easily debuggable
    but usage wise this is as close to using templates as we're gonna get with C99 
*/
void test0(){
    INFO("[%s] : using macro based generic vector types",__FUNCTION__);
    ps_vector_node v_node;
    ps_vector_create(v_node,node);
    ps_vector_push_back(v_node,node_create(gi_count++,0),node);
    ps_vector_push_back(v_node,node_create(gi_count++,1),node);
    ps_vector_push_back(v_node,node_create(gi_count++,2),node);
    display_macro_version(&v_node);
    ps_vector_clear(v_node);
}

int compare_vector_node(
        const void* a, 
        const void* b, 
        size_t data_size
    ){
    return (*(const node*)a).val < (*(const node*)b).val;
}
/*
    performing mergesort on vector_macros version
*/
void test1(){
    INFO("[%s] : using mergesort on generic vector types",__FUNCTION__);
    ps_vector_node v1;
    ps_vector_create(v1,node);
    int data[] = {1,4,3,0,5,45,22};
    for(int i=0;i<ps_count(data);++i){
        ps_vector_push_back(v1,node_create(data[i],0),node);
    }
    display_merge(v1.data,ps_count(data));
    ps_mergesort(v1.data,0,ps_vector_size(v1)-1,sizeof(node),compare_vector_node);
    display_merge(v1.data,ps_count(data));
    ps_vector_pop_back(v1,node);
    ps_vector_pop_back(v1,node);
    display_merge(v1.data,v1.size);
}

/*
    frame rate controller
*/
void test2(){
    INFO("[%s] : timing stuff 60FPS",__FUNCTION__);
    ps_clock_data* c = ps_clock_get();
    ps_clock_start(c);
    while(true){
        while(ps_clock_dt(c) < (double)1.0/60.0){
            
        }
        ps_clock_fps_print(c);
        ps_clock_reset(c);
        if(ps_clock_uptime(c) > 1.0){
            break;
        }
    }
    ps_clock_stop(c);
    INFO("total time : %lfs", ps_clock_uptime(c));

}


double x = 0.0f;
double y = 0.0f;
float shapes_delta[] = {120.0f,90.0f,60.0f,45.0f,30.0f,15.0f,5.0f};
float shapes_time[] = {1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f};
int total_shapes = sizeof(shapes_delta)/sizeof(shapes_delta[0]);

inline void draw_polygon(float delta){
    float angle = 360.0f;
    float radius = 0.5f;
    glBegin(GL_LINE_LOOP);
    glColor3f((GLfloat)0.5f + (GLfloat)sin(x+10), (GLfloat)0.5f + (GLfloat)sin(x+20), (GLfloat)0.5f + (GLfloat)sin(x+30));
    float dx = (float)(random()%100);
    float dy = (float)(random()%100);
    for(float i=0;i<angle;i+=delta){
        glVertex2f((GLfloat)-0.5f + dx/100.0f + radius* (GLfloat)cos(ps_deg2rad(i)), (GLfloat)-0.5 + dy/100.0f + radius* (GLfloat)sin(ps_deg2rad(i)));
    }
    glEnd();
    glFlush();
    x += 0.001f;
}

inline void draw_rectangle(float xmin, float ymin, float w, float h) {
    glColor3f((GLfloat)0.5f + (GLfloat)sin(y + 10), (GLfloat)0.5f + (GLfloat)sin(y + 20), (GLfloat)0.5f + (GLfloat)sin(y + 30));
    //glColor3f(0.0f, 0.8f, 0.6f);
    glBegin(GL_POLYGON);
    glVertex2f((GLfloat)xmin, (GLfloat)ymin);
    glVertex2f((GLfloat)xmin+w, (GLfloat)ymin);
    glVertex2f((GLfloat)xmin+w, (GLfloat)ymin+h);
    glVertex2f((GLfloat)xmin, (GLfloat)ymin+h);
    glEnd();
    glFlush();
    y += 0.001f;
}


/*
    opengl rendering with frame rate control and windowing
*/
void test3(){
    INFO("[%s] : opengl windowing + 60FPS",__FUNCTION__);
    ps_graphics_window* window = ps_graphics_get_window();
    ps_graphics_init(window,APPNAME,800,800);
    bool is_running = true;

    ps_clock_data* t = ps_clock_get();
    ps_clock_data* c = ps_clock_get();
    ps_clock_start(t);
    ps_clock_start(c);

    int mode = 0;
    float delta = shapes_delta[mode];
    float duration = shapes_time[mode];
    char buffer[80];
    while(is_running){
        ps_graphics_window_poll_events(window);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //for (int i = 0; i < 100; ++i) {
        //    draw_polygon(shapes_delta[mode]);
        //}
        float dx = 0.05f;
        float pad = 0.0f;
        for (int r = 0; r < 40; ++r) {
            draw_rectangle(-1.0f+(r*(dx+pad)), -1.0f, 0.05f, rand()%10/20.0f);
        }
        ps_clock_update(c,FPS);
        sprintf_s(buffer, sizeof(buffer), "[%s] (%d FPS)",APPNAME, (int)ceil(1.0 / ps_clock_dt(c)));
        ps_graphics_window_set_title(window, buffer);
        ps_clock_reset(c);
        if(ps_clock_uptime(c) > shapes_time[mode]){
            mode = (mode+1)%total_shapes;
            ps_clock_reset_uptime(c);
        }
        ps_graphics_window_swap_buffers(window);
        is_running = !ps_graphics_window_should_close(window);
    }
    ps_graphics_destroy(window);
    ps_graphics_release_window(window);
    ps_clock_stop(c);
    ps_clock_stop(t);
    INFO("total time : %lfs",ps_clock_uptime(t));
}

/*
    test out event system functionality 
*/
void test4(){
    
}

void (*tests[])() = {test0,test1,test2,test3};

int main(int argc,char** argv){
    for(int i=0;i<ps_count(tests);++i){
        tests[i]();
    }
    return 0;
}