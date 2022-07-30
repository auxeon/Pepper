/**
 @author Abhikalp Unakal
 @date   08 june 2021
 @file   main.c  
 @desc   sandbox launcher and tests
*/

// define the backened for the window management and context creation
#define GLFW
#include "../pch.h"
#include "immintrin.h"

typedef struct node{
    int val;
    int id;
}node;

node node_create(int val, int id){
    return (node){val, id};
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

void display_merge(void* n, ps_size_t size){
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
    int gi_count = 0;
    PS_INFO("[%s] : using macro based generic vector types",__FUNCTION__);
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
        const void* b
    ){
    return (*(const node*)a).val < (*(const node*)b).val;
}
/*
    performing mergesort on vector_macros version
*/
void test1(){
    PS_INFO("[%s] : using mergesort on generic vector types",__FUNCTION__);
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
    (void)getchar();
}

/*
    frame rate controller
*/
void test2(){
    PS_INFO("[%s] : timing stuff 60FPS",__FUNCTION__);
    ps_clock_data* c = ps_clock_get();
    ps_clock_start(c);
    while(true){
        ps_clock_update(c,FPS);
        ps_clock_fps(c);
        ps_clock_fps_print(c);
        ps_clock_dt_print(c);
        ps_clock_reset(c);
        if(ps_clock_uptime(c) > 1.0){
            break;
        }
    }
    ps_clock_stop(c);
    PS_INFO("total time : %lfs", ps_clock_uptime(c));
    (void)getchar();
}


/* r is the reductor to maintain state across function calls */
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

/* r is the reductor to maintain state across function calls */
void draw_rectangle(ps_vec2 bottom_left, ps_vec2 size, ps_color color) {
    const double alpha = ps_clamp(color.a,0.0f,1.0f);
    glColor3f((GLfloat)sin(color.r) * (GLfloat)alpha, (GLfloat)sin(color.g) * (GLfloat)alpha, (GLfloat)sin(color.b) * (GLfloat)alpha);
    glBegin(GL_LINE_LOOP);
    glVertex2f((GLfloat)bottom_left.x, (GLfloat)bottom_left.y);
    glVertex2f((GLfloat)bottom_left.x + (GLfloat)size.w, (GLfloat)bottom_left.y);
    glVertex2f((GLfloat)bottom_left.x + (GLfloat)size.w, (GLfloat)bottom_left.y + (GLfloat)size.h);
    glVertex2f((GLfloat)bottom_left.x, (GLfloat)bottom_left.y + (GLfloat)size.h);
    glEnd();
}
/* draw in the screen space*/
void draw_rect(ps_vec2 bottom_left, ps_vec2 top_right, ps_color color){

    bottom_left.x = (bottom_left.x/(float)APPW)*2.0f - 1.0f;
    bottom_left.y = (bottom_left.y/(float)APPH)*2.0f - 1.0f;
    top_right.x = (top_right.x/(float)APPW)*2.0f - 1.0f;
    top_right.y = (top_right.y/(float)APPH)*2.0f - 1.0f;

    draw_rectangle(
        bottom_left,
        (ps_vec2){top_right.x-bottom_left.x,top_right.y-bottom_left.y},
        color
    );

}

/*
    opengl rendering with frame rate control and windowing
*/
void test3(){
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
        if(ps_window_input_keyboard_istriggered_down(window, PS_KEY_SPACE)){
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

        print_ps_vec2(mouse_position);
        draw_polygon(
            GL_LINE_LOOP,
            mouse_position,
            shapes_delta[total_shapes-1],
            360.0,
            0.05,
            colorz,
            false
        );
        // double dx = 0.05;
        // double pad = 0.0;
        // for (int r = 0; r < 40; ++r) {
        //    draw_rectangle((ps_vec2){.x = -1.0+(r*(dx+pad)), .y = -1.0}, (ps_vec2){.x = 0.05, .y = rand()%10/20.0}, color);
        // }
        ps_clock_update(c,FPS);
        sprintf(buffer, "[%s] [%fx%f] (%0.3lf FPS)",APPNAME, window_size.x, window_size.y, ps_clock_fps(c));
        ps_window_set_title(window, buffer);
        ps_clock_reset(c);
        if(ps_clock_uptime(c) > shapes_time[mode]){
            mode = (mode+1)%(total_shapes);
            ps_clock_reset_uptime(c);
        }
        ps_window_swap_buffers(window);
        is_running = !ps_window_should_close(window);
    }
    ps_window_destroy(window);
    ps_window_release(window);
    ps_clock_stop(c);
    ps_clock_stop(t);
    PS_INFO("total time : %lfs",ps_clock_uptime(t));
    (void)getchar();
}

/*
    test out heap
*/
#define count(x) sizeof(x)/sizeof(x[0])
int cmp_int(const void* a, const void* b) {
    return *((int*)a) > *((int*)b);
}
void print_int(void* vec, ps_size_t n) {
    for(int i = 0; i < n; ++i) {
        printf("%d ", ((int*)vec)[i]);
    }
    printf("\n");
}
void test4(){
    PS_INFO("[%s] : heapify test", __FUNCTION__);
    ps_clock_data* t = ps_clock_get();
    ps_clock_start(t);
    int data[] = { 23, 3, 5, 4, 6, 13, 10, 9, 8, 15, 17 };
    PS_INFO("before heapify : \n");
    print_int(data, count(data));
    make_heap(data, count(data), sizeof(data[0]), cmp_int);
    PS_INFO("after heapify : \n");
    print_int(data, count(data));
    ps_clock_stop(t);
    PS_INFO("total time : %lfs", ps_clock_uptime(t));
    (void)getchar();
}

void test5(){
    const int nbins = 16;
    PS_INFO("[%s] : FFT BARS+ %d",__FUNCTION__,nbins);
    char buffer[80];
    ps_clock_data* t = ps_clock_get();
    ps_clock_start(t);
    double* samples = (double*)malloc(sizeof(double)*nbins);
    if (!samples) {
        fprintf(stderr, "samples malloc failed");
        exit(-1);
    }
    for(int i=0;i<nbins;++i){
        samples[i] = 0.1f;
    }

    ps_window* window = ps_window_get_handle();
    ps_window_init(window,APPNAME,APPW,APPH);

    bool is_running = true;
    while(is_running){
        ps_window_poll_events(window);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        int bars = nbins;
        double dx = 2.0/((double)bars+1.0);
        double pad = 0.0;
        ps_color color = (ps_color){.r=(double)(rand()/100), .g=(double)(rand()/100), .b=(double)(rand()/100), .a=1.0};
        for (int r = 0; r < bars; ++r) {
            draw_rectangle((ps_vec2){.x = -1.0+dx/2+(r*(dx+pad)), .y = -1.0}, (ps_vec2){.x = dx, .y = rand()%50/30.0 + samples[r]}, color);
        }
        ps_clock_update(t,FPS);
        sprintf(buffer, "[%s] (%0.3lf FPS)",APPNAME, ps_clock_fps(t));
        ps_window_set_title(window, buffer);
        ps_clock_reset(t);
        ps_window_swap_buffers(window);
        is_running = !ps_window_should_close(window);
    }

    ps_window_destroy(window);
    ps_window_release(window);

    free(samples);
    ps_clock_stop(t);
    PS_INFO("total time : %lfs", ps_clock_uptime(t));
    (void)getchar();
}


#define MINIAUDIO_IMPLEMENTATION
#include "../lib/miniaudio/miniaudio.h"

#define C_COUNT 2
#define FALSE 0
#define TRUE 1
#define SUCCESS 0
#define MAERROR -1

ps_clock_data* t6;

void data_callback_test6(ma_device* pDevice, void* output, const void* input, ma_uint32 frame_count){
    ma_decoder* pDecoder = (ma_decoder*)pDevice->pUserData;
    ma_uint32 frames_read = (ma_uint32)ma_decoder_read_pcm_frames(pDecoder, output, frame_count);
    for (ma_uint32 i = 0; i < frames_read * C_COUNT; i += 1) {
        ((float*)output)[i] = ((float*)output)[i] * 0.2f;
    }
    // That(void) casting construct is a no - op that makes the compiler unused variable warning go away
    (void)input;
}

void test6(){
    PS_INFO("[%s] : setting up miniaudio for audio playback", __FUNCTION__);
    ps_clock_data* t = ps_clock_get();
    ps_clock_start(t);


    ma_result result;
    ma_decoder decoder;
    ma_device_config deviceConfig;
    ma_device device;

    const char* buffer = "ffdp_wrong_side_of_heaven_cover_abhikalp_unakal.mp3";
    result = ma_decoder_init_file(buffer, NULL, &decoder);
    if (result != SUCCESS) {
        PS_ERROR("decoder failed\n");
        return;
    }

    deviceConfig = ma_device_config_init(ma_device_type_playback);
    deviceConfig.playback.format = decoder.outputFormat;
    deviceConfig.playback.channels = decoder.outputChannels;
    deviceConfig.sampleRate = decoder.outputSampleRate;
    deviceConfig.dataCallback = data_callback_test6;
    deviceConfig.pUserData = &decoder;

    if (ma_device_init(NULL, &deviceConfig, &device) != SUCCESS) {
        PS_ERROR("Failed to open playback device.\n");
        ma_decoder_uninit(&decoder);
        return;
    }

    if (ma_device_start(&device) != SUCCESS) {
        PS_ERROR("Failed to start playback device.\n");
        ma_device_uninit(&device);
        ma_decoder_uninit(&decoder);
        return;
    }
    printf("Playing %s\n", buffer);
    printf("Press Enter to quit...\n");
    int res = getchar();

    ma_device_uninit(&device);
    ma_decoder_uninit(&decoder);

    ps_clock_stop(t);
    PS_INFO("total time : %lfs", ps_clock_uptime(t));
    (void)getchar();
}


/*
For simplicity, this example requires the device to use floating point samples.
*/
#define SAMPLE_FORMAT   ma_format_f32
#define CHANNEL_COUNT   2
#define SAMPLE_RATE     48000


ma_uint32   g_decoderCount;
ma_decoder* g_pDecoders = NULL;
ma_bool32* g_pDecodersAtEnd = NULL;

ma_event g_stopEvent; /* <-- Signaled by the audio thread, waited on by the main thread. */

ma_bool32 are_all_decoders_at_end()
{
    ma_uint32 iDecoder;
    for (iDecoder = 0; iDecoder < g_decoderCount; ++iDecoder) {
        if (g_pDecodersAtEnd[iDecoder] == FALSE) {
            return FALSE;
        }
    }

    return TRUE;
}

ma_uint32 read_and_mix_pcm_frames_f32(ma_decoder* pDecoder, float* pOutputF32, ma_uint32 frameCount)
{
    /*
    The way mixing works is that we just read into a temporary buffer, then take the contents of that buffer and mix it with the
    contents of the output buffer by simply adding the samples together. You could also clip the samples to -1..+1, but I'm not
    doing that in this example.
    */
    float temp[4096];
    ma_uint32 tempCapInFrames = ma_countof(temp) / CHANNEL_COUNT;
    ma_uint32 totalFramesRead = 0;

    while (totalFramesRead < frameCount) {
        ma_uint32 iSample;
        ma_uint32 framesReadThisIteration;
        ma_uint32 totalFramesRemaining = frameCount - totalFramesRead;
        ma_uint32 framesToReadThisIteration = tempCapInFrames;
        if (framesToReadThisIteration > totalFramesRemaining) {
            framesToReadThisIteration = totalFramesRemaining;
        }

        framesReadThisIteration = (ma_uint32)ma_decoder_read_pcm_frames(pDecoder, temp, framesToReadThisIteration);
        if (framesReadThisIteration == 0) {
            break;
        }

        /* Mix the frames together. */
        for (iSample = 0; iSample < framesReadThisIteration * CHANNEL_COUNT; ++iSample) {
            pOutputF32[totalFramesRead * CHANNEL_COUNT + iSample] += temp[iSample];
        }

        totalFramesRead += framesReadThisIteration;

        if (framesReadThisIteration < framesToReadThisIteration) {
            break;  /* Reached EOF. */
        }
    }

    return totalFramesRead;
}

void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
{
    float* pOutputF32 = (float*)pOutput;
    ma_uint32 iDecoder;

    MA_ASSERT(pDevice->playback.format == SAMPLE_FORMAT);   /* <-- Important for this example. */

    for (iDecoder = 0; iDecoder < g_decoderCount; ++iDecoder) {
        if (!g_pDecodersAtEnd[iDecoder]) {
            ma_uint32 framesRead = read_and_mix_pcm_frames_f32(&g_pDecoders[iDecoder], pOutputF32, frameCount);
            if (framesRead < frameCount) {
                g_pDecodersAtEnd[iDecoder] = TRUE;
            }
        }
    }

    /*
    If at the end all of our decoders are at the end we need to stop. We cannot stop the device in the callback. Instead we need to
    signal an event to indicate that it's stopped. The main thread will be waiting on the event, after which it will stop the device.
    */
    if (are_all_decoders_at_end()) {
        ma_event_signal(&g_stopEvent);
    }

    (void)pInput;
}

void test7() {
    PS_INFO("[%s] : miniaudio applying gain to samples", __FUNCTION__);
    ps_clock_data* t = ps_clock_get();
    ps_clock_start(t);

    ma_result result;
    ma_decoder_config decoderConfig;
    ma_device_config deviceConfig;
    ma_device device;
    ma_uint32 iDecoder;

    // play 2 source files and then fade back and forth across them s
    const char* buffer[] = { "ffdp_wrong_side_of_heaven_cover_abhikalp_unakal.mp3", "daft_punk_get_lucky.mp3" };

    g_decoderCount = 2;
    g_pDecoders = (ma_decoder*)malloc(sizeof(*g_pDecoders) * g_decoderCount);
    g_pDecodersAtEnd = (ma_bool32*)malloc(sizeof(*g_pDecodersAtEnd) * g_decoderCount);

    if (!g_pDecoders || !g_pDecodersAtEnd) {
        PS_ERROR("failed to allocate decoder(s)");
        return;
    }

    /* In this example, all decoders need to have the same output format. */
    decoderConfig = ma_decoder_config_init(SAMPLE_FORMAT, CHANNEL_COUNT, SAMPLE_RATE);
    for (iDecoder = 0; iDecoder < g_decoderCount; ++iDecoder) {
        result = ma_decoder_init_file(buffer[iDecoder], &decoderConfig, &g_pDecoders[iDecoder]);
        if (result != SUCCESS) {
            ma_uint32 iDecoder2;
            for (iDecoder2 = 0; iDecoder2 < iDecoder; ++iDecoder2) {
                ma_decoder_uninit(&g_pDecoders[iDecoder2]);
            }
            free(g_pDecoders);
            free(g_pDecodersAtEnd);

            PS_ERROR("Failed to load %s.\n", buffer[iDecoder]);
            return;
        }
        g_pDecodersAtEnd[iDecoder] = FALSE;
    }


    /* Create only a single device. The decoders will be mixed together in the callback. In this example the data format needs to be the same as the decoders. */
    deviceConfig = ma_device_config_init(ma_device_type_playback);
    deviceConfig.playback.format = SAMPLE_FORMAT;
    deviceConfig.playback.channels = CHANNEL_COUNT;
    deviceConfig.sampleRate = SAMPLE_RATE;
    deviceConfig.dataCallback = data_callback;
    deviceConfig.pUserData = NULL;

    if (ma_device_init(NULL, &deviceConfig, &device) != SUCCESS) {
        for (iDecoder = 0; iDecoder < g_decoderCount; ++iDecoder) {
            ma_decoder_uninit(&g_pDecoders[iDecoder]);
        }
        free(g_pDecoders);
        free(g_pDecodersAtEnd);

        PS_ERROR("Failed to open playback device.\n");
    }

    ma_event_init(&g_stopEvent);

    /* Now we start playback and wait for the audio thread to tell us to stop. */
    if (ma_device_start(&device) != SUCCESS) {
        ma_device_uninit(&device);
        for (iDecoder = 0; iDecoder < g_decoderCount; ++iDecoder) {
            ma_decoder_uninit(&g_pDecoders[iDecoder]);
        }
        free(g_pDecoders);
        free(g_pDecodersAtEnd);

        PS_ERROR("Failed to start playback device.\n");
        return;
    }

    printf("Playing [ %s , %s ]\n", buffer[0], buffer[1]);
    PS_INFO("Waiting for playback to complete...\n");

    //ma_event_wait(&g_stopEvent);
    printf("Press Enter to quit...\n");
    int res = getchar();

    /* Getting here means the audio thread has signaled that the device should be stopped. */
    ma_device_uninit(&device);

    for (iDecoder = 0; iDecoder < g_decoderCount; ++iDecoder) {
        ma_decoder_uninit(&g_pDecoders[iDecoder]);
    }
    free(g_pDecoders);
    free(g_pDecodersAtEnd);

    ps_clock_stop(t);
    PS_INFO("total time : %lfs", ps_clock_uptime(t));
    (void)getchar();
}
bool update_fn(int* t, int* i) {
    *i += 1;
    return *i==*t;
}

bool update_noop_fn(int* t, int* i) {
    return *i==*t;
}

int msg_fn(int* t, int* i) {
    char buffer[40];
    int n = sprintf(buffer, "(%d/%d)",*i,*t);
    printf("%s",buffer);
    fflush(stdout);
    return n;
}
void test8() {
    PS_INFO("[%s] : testing out progress bar", __FUNCTION__);
    ps_clock_data* t = ps_clock_get();
    ps_clock_start(t);
    int total = 10;
    int index = 0;
    int nback = 0;
    while(!progress(&total, &index, &nback, update_fn, msg_fn)) {
        while(ps_clock_uptime(t) < 0.2f){
            
        }
        ps_clock_reset_uptime(t);
    }
    ps_clock_stop(t);
    printf("\n");
    PS_INFO("total time : %lfs", ps_clock_uptime(t));
    (void)getchar();
}



void add_rand_arrays_simple(int* __restrict vin_0, int vsz_0, int* __restrict vin_1, int vsz_1, int* __restrict vout_0) {
    for(int i=0; i<vsz_1; ++i) {
        vout_0[i] = vin_0[i] + vin_1[i];
    }
}

void add_rand_arrays_noindexing(int*  vin_0, int vsz_0, int* vin_1, int vsz_1, int*  vout_0) {
    int* const end = vout_0 + vsz_1;
    while(vout_0<end){
        *(vout_0++) = *(vin_0++) + *(vin_1++);
    }
}

void add_rand_arrays_vectorized(int* __restrict vin_0, int vsz_0, int* __restrict vin_1, int vsz_1, int* __restrict vout_0) {
    const int aligned = vsz_1 - vsz_1 % 4;
    for (int i = 0; i < aligned; i+=4) {
        _mm_storeu_si128((__m128i*)&vout_0[i], 
            _mm_add_epi32(
                _mm_loadu_si128((__m128i*)&vin_0[i]), 
                _mm_loadu_si128((__m128i*)&vin_1[i])
            )
        );
    }
    for (int i = aligned; i < vsz_1; ++i) {
        vout_0[i] = vin_0[i] + vin_1[i];
    }
}

int* populate_randints(int vsz_0) {
    int* vout_0 = (int*)malloc(sizeof(int)*vsz_0);
    if (!vout_0) {
        return vout_0;
    }
    for(int i=0; i<vsz_0; ++i) {
        vout_0[i] = rand()%1000;
    }
    return vout_0;
}

void print_array(int* vin_0, int vsz_0) {
    for(int i=0; i<vsz_0; ++i) {
        printf("%d\n",vin_0[i]);
    }
}
void test9() {
    PS_INFO("[%s] : vectorized addition test", __FUNCTION__);
    ps_clock_data* t = ps_clock_get();
    int sz = 1000000000;
    int* v0 = NULL;
    int* v1 = NULL;
    int* v2 = (int*)calloc(sz, sizeof(int));
    v0 = populate_randints(sz);
    v1 = populate_randints(sz);
    ps_clock_start(t);
    // across the vectorized and noindexing versions there is no difference in speed if i used restrict and -O3
    add_rand_arrays_simple(v0,sz,v1,sz,v2);
    // add_rand_arrays_vectorized(v0,sz,v1,sz,v2);
    // add_rand_arrays_noindexing(v0,sz,v1,sz,v2);
    ps_clock_stop(t);
    printf("\n");
    PS_INFO("total time : %lfs", ps_clock_uptime(t));
    printf("\n");
    // print_array(v0,sz);
    // print_array(v1,sz);
    // print_array(v2,sz);
    free(v0);
    free(v1);
    free(v2);
    (void)getchar();
}


double get_iou(ps_vec4 b0, ps_vec4 b1){

    double a1 = (b0.z - b0.x) * (b0.w - b0.y);
    double a2 = (b1.z - b1.x) * (b1.w - b1.y);

    double xx1 = ps_max(b0.x,b1.x);
    double yy1 = ps_max(b0.y,b1.y);
    double xx2 = ps_min(b0.z,b1.z);
    double yy2 = ps_min(b0.w,b1.w);

    double w = ps_max(xx2 - xx1,0.0f);
    double h = ps_max(yy2 - yy1,0.0f);

    double i = w * h;
    double u = a1 + a1 - i;

    double iou = i/u;
    return iou;
}



typedef struct nmsnode{
    int i;
    double c;
} nmsnode;

ps_vector_declare(nmsnode);
ps_vector_declare(ps_vec4);

int nms_cmp(const void* a, const void* b){
    return (*(nmsnode*)a).c < (*(nmsnode*)b).c;
}

void no_op(ps_vector_nmsnode a){

}

nmsnode nmsnode_create(int i, double c){
    return (nmsnode){i,c};
}

/*
    input boxes
    input nboxes
    input confs
    input nconfs
    input confthresh
    input nmsthresh
    output boxindexes
    output nboxindexes
*/
void nmsc(
    ps_vec4* i_b, 
    int i_nb, 
    double* i_c, 
    int i_nc,
    double i_cthresh,
    double i_nmsthresh,
    int* o_index, 
    int* o_nindex
    ){

    ps_vector_nmsnode v0; 
    ps_vector_nmsnode v1; 

    ps_vector_create(v0, nmsnode);
    ps_vector_create(v1, nmsnode);

    // trim all the confs that are less than 0.65
    for(int i=0;i<i_nc;++i){
        if(i_c[i] > i_cthresh){
            ps_vector_push_back(v0, nmsnode_create(i,i_c[i]), nmsnode);
        }
    }

    // sort based on highest confs first
    ps_mergesort(v0.data,0,v0.size-1,sizeof(nmsnode),nms_cmp);

    // dump it all out 
    // for(int i=0;i<v0.size;++i){
    //     printf("%d (%f)\n",i,ps_vector_at(v0,i).c);
    // }

    while(v0.size > 0){
        nmsnode self = ps_vector_at(v0,0);

        ps_vector_push_back(v1, self, nmsnode);

        ps_vector_nmsnode v2;
        ps_vector_create(v2,nmsnode);

        for(int i=1;i<v0.size;++i){
            nmsnode other = ps_vector_at(v0,i);
            double iou = get_iou(i_b[self.i], i_b[other.i]);
            if(iou <= i_nmsthresh){
                ps_vector_push_back(v2, other, nmsnode);
            }
        }

        ps_vector_destroy(v0,no_op);
        v0 = v2;

    }

    ps_vector_destroy(v0,no_op);

    for(int i=0; i<v1.size; ++i){
        o_index[i] = ps_vector_at(v1,i).i;
    }
    *o_nindex = (int)v1.size;
}

/* trying out non maximum suppression */
void test10() {
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

    ps_color color = (ps_color){PS_COLOR_EMERALD, .a=1.0f};

    char buffer[80];
    PS_INFO("%s opengl vendor\n",(char*)glGetString(GL_VENDOR));

    FILE* f = fopen("boxconfs.dat","r");
    int nboxes;

    /* no op (void) to suppress unused return value warning */
    (void)fscanf(f, "%d",&nboxes);
    if (nboxes < 0) {
        return;
    }
    ps_vec4* boxes = (ps_vec4*)malloc(sizeof(ps_vec4)*nboxes);
    double* confs = (double*)malloc(sizeof(double)*nboxes);
    for(int i=0;i<nboxes;++i){
        (void)fscanf(f, "%lf %lf %lf %lf %lf",
        &(boxes[i].x), 
        &(boxes[i].y), 
        &(boxes[i].z), 
        &(boxes[i].w), 
        &confs[i]);
    }
    fclose(f);

    double tmax = 1.0f;
    double tmin = 0.0f;
    double ttime = 2.0f;
    double th = tmin;
    double tdt = (tmax - tmin) / (FPS*ttime);

    int* indx = (int*)malloc(sizeof(int)*nboxes);
    if (!indx) {
        return;
    }
    for(int i=0;i<nboxes;++i){
        indx[i] = -1;
    }
    int nindx = nboxes;

    while(is_running){
        ps_window_poll_events(window);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        nmsc(
            boxes,
            nboxes,
            confs,
            nboxes,
            0.0,
            0.6,
            indx,
            &nindx
        );

        for (int i = 0; i < nindx && i < nboxes; ++i) {
            draw_rect(
                (ps_vec2){boxes[indx[i]].x, boxes[indx[i]].y},
                (ps_vec2){boxes[indx[i]].z, boxes[indx[i]].z},
                (ps_color){color.r,color.g,color.b,1.0 * confs[indx[i]] * confs[indx[i]] * confs[indx[i]]}
            );
         }

        ps_clock_update(c,FPS);
        sprintf(buffer, "[%s] (%0.3lf FPS)",APPNAME, ps_clock_fps(c));
        ps_window_set_title(window, buffer);
        ps_clock_reset(c);
        PS_INFO("nmsthresh : %lf\n",th);
        if(ps_clock_uptime(c) > ttime){
            tdt = -tdt;
            ps_clock_reset_uptime(c);
        }
        th = ps_clamp(th + tdt, 0.0, 1.0);
        ps_window_swap_buffers(window);
        is_running = !ps_window_should_close(window);
    }

    free(indx);
    free(boxes);
    free(confs);

    ps_window_destroy(window);
    ps_window_release(window);
    ps_clock_stop(c);
    ps_clock_stop(t);
    PS_INFO("total time : %lfs",ps_clock_uptime(t));
    (void)getchar();
}



typedef struct customdata{
    ma_decoder* mDecoder;
    double osc01; // pan from left to right
}customdata;

void data_callback_pan_audio(ma_device* pDevice, void* output, const void* input, ma_uint32 frame_count){
    ma_decoder* pDecoder = ((customdata*)(pDevice->pUserData))->mDecoder;
    double osc01 = ((customdata*)(pDevice->pUserData))->osc01;
    ma_uint32 frames_read = (ma_uint32)ma_decoder_read_pcm_frames(pDecoder, output, frame_count);

    for (ma_uint32 i = 0; i < frames_read * C_COUNT; i+=2) {
        ((float*)output)[i + 0] = ((float*)output)[i + 0] * (float)(1.0-osc01);
        ((float*)output)[i + 1] = ((float*)output)[i + 1] * (float)(osc01);
    }
    // That(void) casting construct is a no - op that makes the compiler unused variable warning go away
    (void)input;
}

double lerp01lohi(double lo, double hi, double t){
    return (1.0-t)*lo + (t)*hi;
}


/*
    pan audio from left to right
*/
void test11(){
    if (FPS){
        PS_INFO("[%s] : panning audio left to right + %dFPS",__FUNCTION__,FPS);
    }
    else{
        PS_INFO("[%s] : panning audio left to right + %dFPS",__FUNCTION__,FPS);
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
    double shapes_delta[] = {120.0};
    double shapes_time[] = {1.0};
    int total_shapes = sizeof(shapes_delta)/sizeof(shapes_delta[0]);

    int mode = 0;
    double delta = shapes_delta[mode];
    double duration = shapes_time[mode];
    char buffer[80];
    PS_INFO("%s opengl vendor\n",(char*)glGetString(GL_VENDOR));
    ps_vec2 pos = (ps_vec2){
        .x=0.0,
        .y=0.0
    };
    ps_vec2 prev_pos = (ps_vec2){
        .x=0.0,
        .y=0.0
    };

    // audio stuff here
    ma_result result;
    ma_decoder decoder;
    ma_device_config deviceConfig;
    ma_device device;
    customdata cdata;

    const char* filepath = "ffdp_wrong_side_of_heaven_cover_abhikalp_unakal.mp3";
    result = ma_decoder_init_file(filepath, NULL, &decoder);
    if (result != SUCCESS) {
        PS_ERROR("decoder failed\n");
        return;
    }

    cdata.mDecoder = &decoder;
    cdata.osc01 = 0.0;

    deviceConfig = ma_device_config_init(ma_device_type_playback);
    deviceConfig.playback.format = decoder.outputFormat;
    deviceConfig.playback.channels = decoder.outputChannels;
    deviceConfig.sampleRate = decoder.outputSampleRate;
    deviceConfig.dataCallback = data_callback_pan_audio;
    deviceConfig.pUserData = &cdata;

    if (ma_device_init(NULL, &deviceConfig, &device) != SUCCESS) {
        PS_ERROR("Failed to open playback device.\n");
        ma_decoder_uninit(&decoder);
        return;
    }

    if (ma_device_start(&device) != SUCCESS) {
        PS_ERROR("Failed to start playback device.\n");
        ma_device_uninit(&device);
        ma_decoder_uninit(&decoder);
        return;
    }


    while(is_running){
        // check for any window events here
        ps_window_poll_events(window);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // processing here 


        // rendering here
        double lovol = 0.2;
        double hivol = 0.9;

        cdata.osc01 = lerp01lohi(lovol, hivol, (1.0 + sin(ps_clock_uptime(t)*2))/2.0);
        
        pos = (ps_vec2){
            .x=cdata.osc01 - (hivol+lovol)/2.0,
            .y=0.0
        };

        bool fliph = ps_vec2_sub(pos, prev_pos).x < 0? true: false;
        double xoffrect = fliph?-0.09 : 0.07;
        ps_color color = fliph?color2:color1;

        draw_polygon(
            GL_LINE_LOOP,
            pos,
            shapes_delta[mode],
            360.0, 
            0.05, 
            color,
            fliph
        );

        draw_rectangle(
            (ps_vec2){
                .x = pos.x + xoffrect,
                .y = -0.05
            },
            (ps_vec2){
                .x = 0.02,
                .y = 0.1
            },
            color
        );

        prev_pos = pos;
        
        // clock fps timing stuff here
        ps_clock_update(c,FPS);
        sprintf(buffer, "[%s] (%0.3lf FPS)",APPNAME, ps_clock_fps(c));
        ps_window_set_title(window, buffer);
        ps_clock_reset(c);
        if(ps_clock_uptime(c) > shapes_time[mode]){
            mode = (mode+1)%(total_shapes);
            ps_clock_reset_uptime(c);
        }
        ps_window_swap_buffers(window);
        is_running = !ps_window_should_close(window);
    }

    ma_device_uninit(&device);
    ma_decoder_uninit(&decoder);

    ps_window_destroy(window);
    ps_window_release(window);
    ps_clock_stop(c);
    ps_clock_stop(t);
    PS_INFO("total time : %lfs",ps_clock_uptime(t));
    (void)getchar();
}

void test12() {
    PS_INFO("[%s] : testing out bitsets", __FUNCTION__);
    ps_clock_data* t = ps_clock_get();
    ps_clock_start(t);


    ps_archetype bitfield=0;
    ps_bit64_zro(bitfield);
    ps_bit64_set(bitfield,9);
    ps_bit64_sho(bitfield);
    printf("\n");
    ps_bit64_one(bitfield);
    ps_bit64_sho(bitfield);


    ps_clock_stop(t);
    printf("\n");
    PS_INFO("total time : %lfs", ps_clock_uptime(t));
    printf("\n");
    (void)getchar();
}

void draw_cube() {
    glBegin(GL_LINE_LOOP);
    glColor3f(0.0f, 1.0f, 1.0f);
    glVertex2f((GLfloat)0.1f, (GLfloat)0.1f);
    glVertex2f((GLfloat)0.3f, (GLfloat)0.1f);
    glVertex2f((GLfloat)0.3f, (GLfloat)0.3f);
    glVertex2f((GLfloat)0.1f, (GLfloat)0.3f);
    glEnd();
}

void test13(){
    PS_INFO("[%s] : render 3d cube", __FUNCTION__);
    ps_clock_data* t = ps_clock_get();
    ps_clock_start(t);

    ps_window* window = ps_window_get_handle();
    ps_window_init(window,APPNAME,APPW,APPH);
    char buffer[WINDOW_TITLE_LEN];


    bool is_running = true;
    while(is_running){
        ps_window_poll_events(window);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // write openGL code here
        draw_cube();
        ps_clock_update(t,FPS);
        sprintf(buffer, "[%s] (%0.3lf FPS)",APPNAME, ps_clock_fps(t));
        ps_window_set_title(window, buffer);
        ps_clock_reset(t);
        ps_window_swap_buffers(window);
        is_running = !ps_window_should_close(window);
    }

    ps_window_destroy(window);
    ps_window_release(window);


    ps_clock_stop(t);
    printf("\n");
    PS_INFO("total time : %lfs", ps_clock_uptime(t));
    printf("\n");
    (void)getchar();


}



int main(int argc,char** argv){

    void (*tests[])() = {
        test0,
        test1,
        test2,
        test3,
        test4,
        test5,
        test6,
        test7,
        test8,
        test9,
        test10,
        test11,
        test12,
        test13
    };

    if(argc > 1){
        for(int t=1;t<argc;++t){
            int n = atoi(argv[t]);
            if(n < count(tests)){
                tests[n]();
            }
        }
    }
    else{
        for(int i=0;i<ps_count(tests);++i){
            tests[i]();
        }
    }
    return 0;
}
