/*
@author Abhikalp Unakal
@date   08 june 2021
@file   main.c
@desc   sandbox launcher and tests
*/

#include "../pch.h"
#include "../lib/ps_mergesort.h"
#include "../lib/glad/include/glad/glad.h"
#include "../core/ps_graphics.h"

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
    INFO("total time : %lfs",ps_clock_uptime(c));

}


double x = 0.0f;
float shapes_delta[] = {120.0f,90.0f,60.0f,45.0f,30.0f,15.0f,5.0f};
float shapes_time[] = {1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f};
int total_shapes = sizeof(shapes_delta)/sizeof(shapes_delta[0]);

void draw_polygon(float delta){
    float angle = 360.0f;
    float radius = 0.5f;
    glBegin(GL_LINE_LOOP);
    glColor3f(0.5f + sin(x+10),0.5f + sin(x+20),0.5f + sin(x+30));
    float dx = random()%100;
    float dy = random()%100;
    for(float i=0;i<angle;i+=delta){
        glVertex2f(-0.5f + dx/100.0f + radius*cos(ps_deg2rad(i)),-0.5 + dy/100.0f + radius*sin(ps_deg2rad(i)));
    }
    glEnd();
    glFlush();
    x += 0.01f;
}


/*
    opengl rendering with frame rate control and windowing
*/
void test3(){
    INFO("[%s] : opengl windowing + 60FPS",__FUNCTION__);
    ps_graphics_window window;
    ps_graphics_init(&window,APPNAME,600,600);
    bool is_running = true;

    ps_clock_data* t = ps_clock_get();
    ps_clock_data* c = ps_clock_get();
    ps_clock_start(t);
    ps_clock_start(c);

    int mode = 0;
    float delta = shapes_delta[mode];
    float duration = shapes_time[mode];
    while(!glfwWindowShouldClose(window.window.handle)){
        ps_graphics_window_poll_events(&window);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        draw_polygon(shapes_delta[mode]);
        ps_clock_update(c,60.0);
        ps_clock_fps_print(c);
        ps_clock_reset(c);
        if(ps_clock_uptime(c) > shapes_time[mode]){
            mode = (mode+1)%total_shapes;
            ps_clock_reset_uptime(c);
        }
        ps_graphics_window_swap_buffers(&window);
    }
    ps_graphics_destroy(&window);
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