/*
@author Abhikalp Unakal
@date   08 june 2021
@file   main.c
@desc   sandbox launcher and tests
*/

#include "immintrin.h"

#ifdef _WIN64
#define random rand
#include "Windows.h"
#endif

/*
included Windows.h before glad.h gets included in pch.h/ps_graphics.h to deal with warning C4005: 'APIENTRY': macro redefinition 
*/
#include "../pch.h"
#ifdef _WIN64
// telling optimus to switch to nvidia graphics card instead of internal 
// _declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
#endif

/*
    setting up stuff needed for test0
*/


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
void draw_polygon(ps_vec2 point, double delta, double angle, double radius, ps_color color){
    glBegin(GL_LINE_LOOP);
    double alpha = ps_clamp(color.a,0.0f,1.0f);
    glColor3f((GLfloat)sin(color.r) * (GLfloat)alpha, (GLfloat)sin(color.g) * (GLfloat)alpha, (GLfloat)sin(color.b) * (GLfloat)alpha);
    const double dx = 0.0f;
    const double dy = 0.0f;
    for(double i=0;i<angle;i+=delta){
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
    
    ps_graphics_window* window = ps_graphics_get_window();
    ps_graphics_init(window,APPNAME,APPW,APPH);
    bool is_running = true;

    ps_color color = (ps_color){PS_COLOR_EMERALD, .a=1.0f};
    double shapes_delta[] = {120.0,90.0,60.0,45.0,30.0,15.0,5.0};
    double shapes_time[] = {1.0,1.0,1.0,1.0,1.0,1.0,1.0};
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
    while(is_running){
        ps_graphics_window_poll_events(window);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        for (unsigned int i = 1; i < 10; ++i) {

            pos = (ps_vec2){
                .x=cos(ps_clock_uptime(t)*(i+1)*0.5 + 10*i*PI/180.0f)*0.2,
                .y=sin(ps_clock_uptime(t)*(i+1)*0.5 + 10*i*PI/180.0f)*0.2
            };

            draw_polygon(
                pos,
                shapes_delta[mode],
                360.0, 
                0.05, 
                color
            );
         }
        // double dx = 0.05;
        // double pad = 0.0;
        // for (int r = 0; r < 40; ++r) {
        //    draw_rectangle((ps_vec2){.x = -1.0+(r*(dx+pad)), .y = -1.0}, (ps_vec2){.x = 0.05, .y = rand()%10/20.0}, color);
        // }
        ps_clock_update(c,FPS);
        sprintf(buffer, "[%s] (%0.3lf FPS)",APPNAME, ps_clock_fps(c));
        ps_graphics_window_set_title(window, buffer);
        ps_clock_reset(c);
        if(ps_clock_uptime(c) > shapes_time[mode]){
            mode = (mode+1)%(total_shapes);
            ps_clock_reset_uptime(c);
        }
        ps_graphics_window_swap_buffers(window);
        is_running = !ps_graphics_window_should_close(window);
    }
    ps_graphics_destroy(window);
    ps_graphics_release_window(window);
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

    ps_graphics_window* window = ps_graphics_get_window();
    ps_graphics_init(window,APPNAME,APPW,APPH);

    bool is_running = true;
    while(is_running){
        ps_graphics_window_poll_events(window);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        int bars = nbins;
        double dx = 2.0f/(double)(bars+1);
        double pad = 0.0f;
        ps_color color = (ps_color){.r=(double)(rand()/100), .g=(double)(rand()/100), .b=(double)(rand()/100), .a=1.0f};
        for (int r = 0; r < bars; ++r) {
            draw_rectangle((ps_vec2){.x = -1.0f+dx/2+(r*(dx+pad)), .y = -1.0f}, (ps_vec2){.x = dx, .y = rand()%50/30.0f + samples[r]}, color);
        }
        ps_clock_update(t,FPS);
        sprintf(buffer, "[%s] (%0.3lf FPS)",APPNAME, ps_clock_fps(t));
        ps_graphics_window_set_title(window, buffer);
        ps_clock_reset(t);
        ps_graphics_window_swap_buffers(window);
        is_running = !ps_graphics_window_should_close(window);
    }

    ps_graphics_destroy(window);
    ps_graphics_release_window(window);

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
    printf("Press Enter to quit...");
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
ma_decoder* g_pDecoders;
ma_bool32* g_pDecodersAtEnd;

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

    ma_event_wait(&g_stopEvent);

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



void add_rand_arrays_simple(int* vin_0, int vsz_0, int* vin_1, int vsz_1, int* vout_0) {
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
    // add_rand_arrays_simple(v0,sz,v1,sz,v2);
    // add_rand_arrays_vectorized(v0,sz,v1,sz,v2);
    add_rand_arrays_noindexing(v0,sz,v1,sz,v2);
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
}

void test10() {
    return;
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
        test9
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
