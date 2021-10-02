/*
@author Abhikalp Unakal
@date   08 june 2021
@file   main.c
@desc   sandbox launcher and tests
*/

#ifdef _WIN64
#define random rand
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "../pch.h"

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
    getchar();
}

/*
    frame rate controller
*/
void test2(){
    PS_INFO("[%s] : timing stuff 60FPS",__FUNCTION__);
    ps_clock_data* c = ps_clock_get();
    ps_clock_start(c);
    while(true){
        while(ps_clock_dt(c) < (double)1.0/FPS){
            
        }
        ps_clock_fps_print(c);
        ps_clock_reset(c);
        if(ps_clock_uptime(c) > 1.0){
            break;
        }
    }
    ps_clock_stop(c);
    PS_INFO("total time : %lfs", ps_clock_uptime(c));
    getchar();
}


double x = 0.0f;
double y = 0.0f;
float shapes_delta[] = {120.0f,90.0f,60.0f,45.0f,30.0f,15.0f,5.0f};
float shapes_time[] = {1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f};
int total_shapes = sizeof(shapes_delta)/sizeof(shapes_delta[0]);

void draw_polygon(float delta){
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
    //glFlush();
    x += 0.001f;
}

void draw_rectangle(float xmin, float ymin, float w, float h) {
    glColor3f((GLfloat)0.5f + (GLfloat)sin(y + 10), (GLfloat)0.5f + (GLfloat)sin(y + 20), (GLfloat)0.5f + (GLfloat)sin(y + 30));
    glBegin(GL_LINE_LOOP);
    glVertex2f((GLfloat)xmin, (GLfloat)ymin);
    glVertex2f((GLfloat)xmin+w, (GLfloat)ymin);
    glVertex2f((GLfloat)xmin+w, (GLfloat)ymin+h);
    glVertex2f((GLfloat)xmin, (GLfloat)ymin+h);
    glEnd();
    //glFlush();
    y += 0.001f;
}


/*
    opengl rendering with frame rate control and windowing
*/
void test3(){
    PS_INFO("[%s] : opengl windowing + %dFPS",__FUNCTION__,FPS);

    ps_clock_data* t = ps_clock_get();
    ps_clock_data* c = ps_clock_get();
    ps_clock_start(t);
    ps_clock_start(c);
    
    ps_graphics_window* window = ps_graphics_get_window();
    ps_graphics_init(window,APPNAME,APPW,APPH);
    bool is_running = true;

    int mode = 0;
    float delta = shapes_delta[mode];
    float duration = shapes_time[mode];
    char buffer[80];
    while(is_running){
        ps_graphics_window_poll_events(window);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
         for (int i = 0; i < 1; ++i) {
            draw_polygon(shapes_delta[mode]);
         }
        //float dx = 0.05f;
        //float pad = 0.0f;
        //for (int r = 0; r < 40; ++r) {
        //    draw_rectangle(-1.0f+(r*(dx+pad)), -1.0f, 0.05f, rand()%10/20.0f);
        //}
        ps_clock_update(c,FPS);
        sprintf(buffer, "[%s] (%d FPS)",APPNAME, (int)ceil(1.0 / ps_clock_dt(c)));
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
    PS_INFO("total time : %lfs",ps_clock_uptime(t));
    getchar();
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
    getchar();
}

void test5(){
    const int nbins = 16;
    PS_INFO("[%s] : FFT BARS+ %d",__FUNCTION__,nbins);
    char buffer[80];
    ps_clock_data* t = ps_clock_get();
    ps_clock_start(t);
    float* samples = (float*)malloc(sizeof(float)*nbins);
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
        float dx = 2.0f/(float)(bars+1);
        float pad = 0.0f;
        for (int r = 0; r < bars; ++r) {
            draw_rectangle(-1.0f+dx/2+(r*(dx+pad)), -1.0f, dx, rand()%50/30.0f + samples[r]);
        }

        ps_clock_update(t,FPS);
        sprintf(buffer, "[%s] (%d FPS)",APPNAME, (int)ceil(1.0 / ps_clock_dt(t)));
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
    getchar();
}


#define MINIAUDIO_IMPLEMENTATION
#include "../lib/miniaudio/miniaudio.h"

#define C_COUNT 2

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
    t6 = t;
    ps_clock_start(t);


    ma_result result;
    ma_decoder decoder;
    ma_device_config deviceConfig;
    ma_device device;

    const char* buffer = "../../sandbox/ffdp_wrong_side_of_heaven_cover_abhikalp_unakal.mp3";
    result = ma_decoder_init_file(buffer, NULL, &decoder);
    if (result != MA_SUCCESS) {
        PS_ERROR("decoder failed\n");
        return;
    }

    deviceConfig = ma_device_config_init(ma_device_type_playback);
    deviceConfig.playback.format = decoder.outputFormat;
    deviceConfig.playback.channels = decoder.outputChannels;
    deviceConfig.sampleRate = decoder.outputSampleRate;
    deviceConfig.dataCallback = data_callback_test6;
    deviceConfig.pUserData = &decoder;

    if (ma_device_init(NULL, &deviceConfig, &device) != MA_SUCCESS) {
        PS_ERROR("Failed to open playback device.\n");
        ma_decoder_uninit(&decoder);
        return;
    }

    if (ma_device_start(&device) != MA_SUCCESS) {
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
    getchar();
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
        if (g_pDecodersAtEnd[iDecoder] == MA_FALSE) {
            return MA_FALSE;
        }
    }

    return MA_TRUE;
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
                g_pDecodersAtEnd[iDecoder] = MA_TRUE;
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
    const char* buffer[] = { "../../sandbox/ffdp_wrong_side_of_heaven_cover_abhikalp_unakal.mp3", "../../sandbox/daft_punk_get_lucky.mp3" };

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
        if (result != MA_SUCCESS) {
            ma_uint32 iDecoder2;
            for (iDecoder2 = 0; iDecoder2 < iDecoder; ++iDecoder2) {
                ma_decoder_uninit(&g_pDecoders[iDecoder2]);
            }
            free(g_pDecoders);
            free(g_pDecodersAtEnd);

            PS_ERROR("Failed to load %s.\n", buffer[iDecoder]);
            return;
        }
        g_pDecodersAtEnd[iDecoder] = MA_FALSE;
    }


    /* Create only a single device. The decoders will be mixed together in the callback. In this example the data format needs to be the same as the decoders. */
    deviceConfig = ma_device_config_init(ma_device_type_playback);
    deviceConfig.playback.format = SAMPLE_FORMAT;
    deviceConfig.playback.channels = CHANNEL_COUNT;
    deviceConfig.sampleRate = SAMPLE_RATE;
    deviceConfig.dataCallback = data_callback;
    deviceConfig.pUserData = NULL;

    if (ma_device_init(NULL, &deviceConfig, &device) != MA_SUCCESS) {
        for (iDecoder = 0; iDecoder < g_decoderCount; ++iDecoder) {
            ma_decoder_uninit(&g_pDecoders[iDecoder]);
        }
        free(g_pDecoders);
        free(g_pDecodersAtEnd);

        PS_ERROR("Failed to open playback device.\n");
    }

    ma_event_init(&g_stopEvent);

    /* Now we start playback and wait for the audio thread to tell us to stop. */
    if (ma_device_start(&device) != MA_SUCCESS) {
        ma_device_uninit(&device);
        for (iDecoder = 0; iDecoder < g_decoderCount; ++iDecoder) {
            ma_decoder_uninit(&g_pDecoders[iDecoder]);
        }
        free(g_pDecoders);
        free(g_pDecodersAtEnd);

        PS_ERROR("Failed to start playback device.\n");
        return;
    }

    PS_ERROR("Waiting for playback to complete...\n");
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
    getchar();
}

void (*tests[])() = {
    test0,
    test1,
    test2,
    test3,
    test4,
    test5,
    test6,
    test7
};

int main(int argc,char** argv){
    for(int i=0;i<ps_count(tests);++i){
        tests[i]();
    }
    return 0;
}
