//
//  AppDelegate.m
//  FrogFu
//
//  Created by Vladimir Bodurov on 12-08-04.
//  Copyright (c) 2012 Vladimir Bodurov. All rights reserved.
//
#import "Interfaces.hpp"
#import "Factory.hpp"
#import "AppDelegate.h"

#import "MainViewController.h"
#import "GameOpenAL.h"
#import "GLView.h"
#import <mach/mach.h>
#import <mach/mach_host.h>
#import <MessageUI/MessageUI.h>
#import <MessageUI/MFMailComposeViewController.h>
#import <Social/Social.h>


#define CHOICE_RATE 1
#define CHOICE_NO 0
#define CHOICE_LATER 2

@implementation AppDelegate{
    GLView* _glView;
}

@synthesize window;
@synthesize bgWorkerState;


std::ostream& operator<<(std::ostream& os, const vec2& v) {
    return v.out(os);
}
double ensureRange(double value, double min, double max){
    if(value < min) return min;
    if(value > max) return max;
    return value;
}
double sysRandom(){
    return rand() / (double)RAND_MAX;
}
double sysRandomOfPeriodAsDouble(double min, double max){
    double diff = max - min;
    return diff * sysRandom() + min;
}
int sysRandomOfPeriodAsInt(int min, int max){
    int diff = max - min + 1;
    return diff * sysRandom() + min;
}
void backgroundWorker(void (*work)(void*), void(*done)(void*), void* caller){
    BgWorkerState* bws = new BgWorkerState();
    bws->Caller = caller;
    bws->Work = work;
    bws->Done = done;
    [[AppDelegate instance] startBgWorker:bws];
}
void analyticsEvent(const char* action, const char* label, long value){
    NSString* cat = [[NSString alloc] initWithUTF8String:"navigation"];
    NSString* act = [[NSString alloc] initWithUTF8String:action];
    NSString* lbl = [[NSString alloc] initWithUTF8String:label];
    NSNumber* num = [NSNumber numberWithInt:value];
    [[AppDelegate instance] analyticsEventsWithCategory:cat withAction:act withLabel:lbl withValue:num];
}
void analyticsScreen(const char* screen){
    NSString* scr = [[NSString alloc] initWithUTF8String:screen];
    [[AppDelegate instance] analyticsScreen:scr];
}
void submitScore(long score, const char* leaderboard){
    NSString* lb = [[NSString alloc] initWithUTF8String:leaderboard];
    [[AppDelegate instance] submitScore:score ForLeaderboard:lb];
}
void submitAchievement(const char* achievement){
    NSString* ach = [[NSString alloc] initWithUTF8String:achievement];
    [[AppDelegate instance] submitAchievement:ach];
}
void showLeaderboards(){
    [[AppDelegate instance] showLeaderboards];
}
void showAchievements(){
    [[AppDelegate instance] showAchievements];
}
void postToTwitter(long score, int type){
    [[AppDelegate instance] postToTwitterScore:score withType:type];
}
void postToFacebook(long score, int type){
    [[AppDelegate instance] postToFacebookScore:score withType:type];
}
void postToGooglePlus(long score, int type){
    [[AppDelegate instance] postToGooglePlusScore:score withType:type];
}

int getFreeMemory(){
    return (int)([AppDelegate getFreeMemory]/(1024.0*1024.0));
}

void sysSendEmail(){
    [[UIApplication sharedApplication] openURL: [NSURL URLWithString:@"mailto:info@youvisio.com?subject=Frog-Fu"]];
}
void sysGoToUrl(const char* url){
    NSString* nsurl = [[NSString alloc] initWithUTF8String:url];
    [[UIApplication sharedApplication] openURL: [NSURL URLWithString:nsurl] ];
}


void sysLoadSound(const char* name, const char* ext, bool loop, bool isMusic){
    NSString* nsname = [[NSString alloc] initWithUTF8String:name];
    NSString* nsext = [[NSString alloc] initWithUTF8String:ext];
    SoundInfo* si = (*Factory::Instance->SoundInfos)[name];
    if(si == NULL){
        SoundInfo* si = new SoundInfo();
        si->name = name;
        si->ext = ext;
        si->loop = loop;
        si->isPlaying = false;
        si->isMusic = isMusic;
        (*Factory::Instance->SoundInfos)[name] = si;
    }
    [[GameOpenAL instance] loadSoundWithKey:nsname File:nsname Ext:nsext Loop:loop];
}
void sysPlaySound(const char* name){
    NSString* nsname = [[NSString alloc] initWithUTF8String:name];
    
    SoundInfo* si = (*Factory::Instance->SoundInfos)[name];
    if(si != NULL){
        if(!si->isMusic && !Factory::Instance->Settings->Sounds()){
            return;
        }
        
        si->isPlaying = true;
        [[GameOpenAL instance] playSoundWithKey:nsname];
    }
}
void sysStopSound(const char* name){
    NSString* nsname = [[NSString alloc] initWithUTF8String:name];
    
    SoundInfo* si = (*Factory::Instance->SoundInfos)[name];
    if(si != NULL){
        si->isPlaying = false;
        [[GameOpenAL instance] stopSoundWithKey:nsname];
    }
}
void sysRewindSound(const char* name){
    NSString* nsname = [[NSString alloc] initWithUTF8String:name];
    [[GameOpenAL instance] rewindSoundWithKey:nsname];
}
void sysSoundVolume(const char* name, float volume){
    NSString* nsname = [[NSString alloc] initWithUTF8String:name];
    SoundInfo* si = (*Factory::Instance->SoundInfos)[name];
    if(si != NULL){
        if(si->isMusic && !Factory::Instance->Settings->Music()){
            if(si->volume == 0.0) return;
            volume = 0.0;
        } else if(!si->isMusic && !Factory::Instance->Settings->Sounds()){
            if(si->volume == 0.0) return;
            volume = 0.0;
        }
        
        si->volume = volume;
        [[GameOpenAL instance] changeSoundVolumeWithKey:nsname Volume:volume];
    }
}
bool sysIsPlayingSound(const char* name){
    NSString* nsname = [[NSString alloc] initWithUTF8String:name];
    return [[GameOpenAL instance] isPlayingSoundWithKey:nsname];
}

void showRatingAlert(){
    [[AppDelegate instance] showRatingAlert];
}


static AppDelegate* _instance;
+ (AppDelegate*)instance{
    return _instance;
}
- (void)showRatingAlert{
    UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:APPIRATER_MESSAGE_TITLE
                                                        message:APPIRATER_MESSAGE
                                                       delegate:self
                                              cancelButtonTitle:APPIRATER_CANCEL_BUTTON
                                              otherButtonTitles:APPIRATER_RATE_BUTTON, APPIRATER_RATE_LATER, nil];
    [alertView show];
    
}
- (void)alertView:(UIAlertView *)alertView didDismissWithButtonIndex:(NSInteger)buttonIndex {
    if(buttonIndex == CHOICE_RATE || buttonIndex == CHOICE_NO){
        Factory::Instance->Settings->WarnToRate(false);
        analyticsScreen("doNotRate");
    }
    if(buttonIndex == CHOICE_LATER){
        Factory::Instance->skipWarnToRate = 3;
        analyticsScreen("laterRate");
    }
    if(buttonIndex == CHOICE_RATE){
        [self rateApp];
        analyticsScreen("rate");
    }
}

- (void)rateApp {
    
#if TARGET_IPHONE_SIMULATOR
    NSLog(@"APPIRATER NOTE: iTunes App Store is not supported on the iOS simulator. Unable to open App Store page.");
#else
    NSString *reviewURL = [_templateReviewURL stringByReplacingOccurrencesOfString:@"APP_ID" withString:[NSString stringWithFormat:@"%@", _appId]];
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:reviewURL]];
#endif
    
}

- (void)submitScore:(long)score ForLeaderboard:(NSString*)leaderboard{
    GKScore *myScoreValue = [[GKScore alloc] initWithCategory:leaderboard];
    myScoreValue.value = score;
    
    [myScoreValue reportScoreWithCompletionHandler:^(NSError *error){
        if(error != nil){
            NSLog(@"Score Submission Failed score: %ld leaderboard: %@", score, leaderboard);
        } else {
            NSLog(@"Score Submitted score: %ld leaderboard: %@", score, leaderboard);
        }
        
    }];
}
- (void)submitAchievement:(NSString*)achievementName{
    GKAchievement *achievement= [[GKAchievement alloc] initWithIdentifier:achievementName];
    if(achievement!= NULL)
    {
        achievement.percentComplete = 100.0;
        [achievement reportAchievementWithCompletionHandler: ^(NSError *error){
            if(error != nil){
                NSLog(@"Achievement faile %@", achievementName);
            } else {
                NSLog(@"Achievement Succes %@", achievementName);
            }
            
        }];
    }
}
- (void)showLeaderboards{
    GKLeaderboardViewController *lb = [[GKLeaderboardViewController alloc] init];
    if(lb != nil){
        lb.leaderboardDelegate = self;
        [self.window.rootViewController presentModalViewController:lb animated:YES];
    }
}
- (void)leaderboardViewControllerDidFinish:(GKLeaderboardViewController *)viewController
{
    [self.window.rootViewController dismissModalViewControllerAnimated: YES];
}

- (void)showAchievements{
    GKAchievementViewController *achievements = [[GKAchievementViewController alloc] init];
    if (achievements != NULL)
    {
        achievements.achievementDelegate = self;
        [self.window.rootViewController presentModalViewController:achievements animated: YES];
    }
}
- (void)achievementViewControllerDidFinish:(GKAchievementViewController *)viewController;
{
    [self.window.rootViewController dismissModalViewControllerAnimated: YES];
}
- (void)analyticsEventsWithCategory:(NSString*)category
                         withAction:(NSString*)action
                          withLabel:(NSString*)label
                          withValue:(NSNumber*)value
{
    [self.tracker sendEventWithCategory:category
                             withAction:action
                              withLabel:label
                              withValue:value];
}
- (void)analyticsScreen:(NSString*)screen
{
    [self.tracker sendView:screen];
}
- (void)postToTwitterScore:(long)score
                  withType:(int)type
{
    if (Factory::Instance->iOSversion >= 6 &&
       [SLComposeViewController isAvailableForServiceType:SLServiceTypeTwitter])
    {
        Factory::Instance->canPostToSocial = false;
        
        SLComposeViewController *tweetSheet = [SLComposeViewController composeViewControllerForServiceType: SLServiceTypeTwitter];

        NSString *txt = [NSString stringWithFormat:@"I just scored %ld points in the game Frog Fu", score];
        
        [tweetSheet setInitialText:txt];
        [tweetSheet addImage:[UIImage imageNamed:@"Frog.png"]];
        [tweetSheet addURL:[NSURL URLWithString:@"https://itunes.apple.com/app/frog-fu/id574269225"]];
        
        [tweetSheet setCompletionHandler:^(SLComposeViewControllerResult result) {
            Factory::Instance->canPostToSocial = true;
            switch (result) {
                case SLComposeViewControllerResultCancelled:
                    NSLog(@"Twitter Post Canceled");
                    break;
                case SLComposeViewControllerResultDone:
                    NSLog(@"Twitter Post Sucessful");
                    break;
                    
                default:
                    break;
            }
        }];
        
        [self.window.rootViewController  presentViewController:tweetSheet animated:YES completion:nil];
        
        analyticsScreen("twitterPost");
    }
    else
    {
        [[UIApplication sharedApplication] openURL: [NSURL URLWithString:@"https://twitter.com/YouVisio"] ];
        
        analyticsScreen("twitterLink");
    }
}
- (void)postToFacebookScore:(long)score
                   withType:(int)type
{
    if (Factory::Instance->iOSversion >= 6 &&
        [SLComposeViewController isAvailableForServiceType:SLServiceTypeFacebook])
    {
        Factory::Instance->canPostToSocial = false;
        
        SLComposeViewController *facebook = [SLComposeViewController composeViewControllerForServiceType: SLServiceTypeFacebook];
        
        NSString *txt = [NSString stringWithFormat:@"I just scored %ld points in the game Frog Fu", score];
        
        [facebook setInitialText:txt];
        [facebook addURL:[NSURL URLWithString:@"https://itunes.apple.com/app/frog-fu/id574269225"]];
        
        [facebook setCompletionHandler:^(SLComposeViewControllerResult result) {
            Factory::Instance->canPostToSocial = true;
            switch (result) {
                case SLComposeViewControllerResultCancelled:
                    NSLog(@"Facebook Post Canceled");
                    break;
                case SLComposeViewControllerResultDone:
                    NSLog(@"Facebook Post Sucessful");
                    break;
                    
                default:
                    break;
            }
        }];
        
        [self.window.rootViewController  presentViewController:facebook animated:YES completion:nil];
        
        analyticsScreen("facebookPost");
    }
    else
    {
        [[UIApplication sharedApplication] openURL: [NSURL URLWithString:@"https://www.facebook.com/YouVisio"] ];
        
        analyticsScreen("facebookLink");
    }
}
- (void)postToGooglePlusScore:(long)score
                     withType:(int)type
{
    
}

- (void)startBgWorker:(void*)bws{
    bgWorkerState = bws;
    NSOperationQueue *queue = [NSOperationQueue new];
    NSInvocationOperation *operation = [[NSInvocationOperation alloc]
                                        initWithTarget:self
                                        selector:@selector(executeBgWork)
                                        object:nil];
    [queue addOperation:operation];
}
- (void)executeBgWork{
    BgWorkerState* bws = ((BgWorkerState*)bgWorkerState);
    bws->Work(bws->Caller);
    [self performSelectorOnMainThread:@selector(onBgWorkDone:) withObject:nil waitUntilDone:NO];
}
- (void)onBgWorkDone:(void*)obj{
    BgWorkerState* bws = ((BgWorkerState*)bgWorkerState);
    bws->Done(bws->Caller);
    delete bws;
    bgWorkerState = NULL;
}
- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    _instance = self;
    
    _appId = @"574269225";
    _templateReviewURL = @"itms-apps://ax.itunes.apple.com/WebObjects/MZStore.woa/wa/viewContentsUserReviews?type=Purple+Software&id=574269225";

    
    CGRect screenBounds = [[UIScreen mainScreen] bounds];
    
    self.window = [[UIWindow alloc] initWithFrame:screenBounds];
    
    
//cout << "W:" << screenBounds.size.width << " H:" << screenBounds.size.height << endl;
    int iPhone = 3;
    //isIpad = true; /* Device is iPad */
    bool isIpad = [[[UIDevice currentDevice] model] isEqualToString:@"iPad"] ||
                  [[[UIDevice currentDevice] model] isEqualToString:@"iPad Simulator"];
    
    bool isSimulator = false;
    
    /*if(isIpad){
//        screenBounds.size.width *= 1.2;
//        screenBounds.size.height *= 1.2;
    }else*/{
        
        double side = sysMax(screenBounds.size.height, screenBounds.size.width);
    
#if TARGET_IPAD_SIMULATOR
      isSimulator = true;  
#endif
#if TARGET_IPHONE_SIMULATOR
        
        // Simulator specific code
        // iPhone 4 = 2 times bigger
        if(side > 470.0 && side < 490.0){
            iPhone = 4;
        }
        // iPhone 5 = 4 times bigger
        if(side > 560.0f && side < 580.0f){
            iPhone = 5;
        }
        isSimulator = true;
        
#else // TARGET_IPHONE_SIMULATOR
        
        // Device specific code
        // iPhone 4 = 2 times bigger
        if(side > 470.0 && side < 490.0){
            iPhone = 4;
            screenBounds.size.width *= 1.4142;
            screenBounds.size.height *= 1.4142;
        }
        // iPhone 5 = 4 times bigger
        if(side > 560.0f && side < 580.0f){
            iPhone = 5;
            screenBounds.size.width *= 2;
            screenBounds.size.height *= 2;
        }
#endif
    }
    
        
    
    MainViewController * controller = [[MainViewController alloc] init];
    
    self.window.rootViewController = controller;
    
    [self.window setUserInteractionEnabled:YES];
	[self.window setMultipleTouchEnabled:YES];
    
    
    


    GLView* view = [[GLView alloc] initWithFrame:screenBounds forIphone:iPhone isIpad:isIpad onSimulator:isSimulator];
    
    _glView = view;
    
    [controller setUiView:view isIpad:isIpad];
    
    //self.window.backgroundColor = [UIColor whiteColor];
    [self.window addSubview:view];
    [self.window makeKeyAndVisible];
    
    if(![GKLocalPlayer localPlayer].authenticated){
        [[GKLocalPlayer localPlayer] authenticateWithCompletionHandler:^(NSError *error){
            if (error == nil) {
                NSLog(@"Success Loading Game Center");
            } else {
                NSLog(@"Fail Loading Game Center '%@' '%@'", [error debugDescription], [error localizedDescription]);
            }
            
        }];
    }
    
    //[GAI sharedInstance].debug = YES;
    [GAI sharedInstance].dispatchInterval = 20;
//    [GAI sharedInstance].trackUncaughtExceptions = YES;
    self.tracker = [[GAI sharedInstance] trackerWithTrackingId:@"UA-37870528-1"];

    
    return YES;
}



+ (natural_t) getFreeMemory {
    mach_port_t host_port;
    mach_msg_type_number_t host_size;
    vm_size_t pagesize;
    host_port = mach_host_self();
    host_size = sizeof(vm_statistics_data_t) / sizeof(integer_t);
    host_page_size(host_port, &pagesize);
    vm_statistics_data_t vm_stat;
    
    if (host_statistics(host_port, HOST_VM_INFO, (host_info_t)&vm_stat, &host_size) != KERN_SUCCESS) {
        NSLog(@"Failed to fetch vm statistics");
        return 0;
    }
    
    /* Stats in bytes */
    natural_t mem_free = vm_stat.free_count * pagesize;
    return mem_free;
}

- (void)applicationWillResignActive:(UIApplication *)application
{
    // Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
    // Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
    if(_glView != nil) [_glView applicationWillResignActive: application];
}

- (void)applicationDidEnterBackground:(UIApplication *)application
{
    // Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later. 
    // If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
    if(_glView != nil) [_glView applicationDidEnterBackground: application];
}

- (void)applicationWillEnterForeground:(UIApplication *)application
{
    // Called as part of the transition from the background to the inactive state; here you can undo many of the changes made on entering the background.
    if(_glView != nil) [_glView applicationWillEnterForeground: application];
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
    // Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
    if(_glView != nil) [_glView applicationDidBecomeActive: application];
}

- (void)applicationWillTerminate:(UIApplication *)application
{
    // Called when the application is about to terminate. Save data if appropriate. See also applicationDidEnterBackground:.
    if(_glView != nil) [_glView applicationWillTerminate: application];
}

@end
