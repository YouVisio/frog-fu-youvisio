//
//  AppDelegate.h
//  FrogFu
//
//  Created by Vladimir Bodurov on 12-08-04.
//  Copyright (c) 2012 Vladimir Bodurov. All rights reserved.
//

#import <UIKit/UIKit.h>

#import <StoreKit/StoreKit.h>
#import <GameKit/GameKit.h>
#import "GAI.h"

/*
 Your localized app's name.
 */
#define APPIRATER_LOCALIZED_APP_NAME    [[[NSBundle mainBundle] localizedInfoDictionary] objectForKey:@"CFBundleDisplayName"]

/*
 Your app's name.
 */
#define APPIRATER_APP_NAME				APPIRATER_LOCALIZED_APP_NAME ? APPIRATER_LOCALIZED_APP_NAME : [[[NSBundle mainBundle] infoDictionary] objectForKey:(NSString*)kCFBundleNameKey]

/*
 This is the message your users will see once they've passed the day+launches
 threshold.
 */
#define APPIRATER_LOCALIZED_MESSAGE     NSLocalizedStringFromTable(@"If you enjoy using %@, would you mind taking a moment to rate it? It won't take more than a minute. Thanks for your support!", @"AppiraterLocalizable", nil)
#define APPIRATER_MESSAGE				[NSString stringWithFormat:APPIRATER_LOCALIZED_MESSAGE, APPIRATER_APP_NAME]

/*
 This is the title of the message alert that users will see.
 */
#define APPIRATER_LOCALIZED_MESSAGE_TITLE   NSLocalizedStringFromTable(@"Rate %@", @"AppiraterLocalizable", nil)
#define APPIRATER_MESSAGE_TITLE             [NSString stringWithFormat:APPIRATER_LOCALIZED_MESSAGE_TITLE, APPIRATER_APP_NAME]

/*
 The text of the button that rejects reviewing the app.
 */
#define APPIRATER_CANCEL_BUTTON			NSLocalizedStringFromTable(@"No, Thanks", @"AppiraterLocalizable", nil)

/*
 Text of button that will send user to app review page.
 */
#define APPIRATER_LOCALIZED_RATE_BUTTON NSLocalizedStringFromTable(@"Rate %@", @"AppiraterLocalizable", nil)
#define APPIRATER_RATE_BUTTON			[NSString stringWithFormat:APPIRATER_LOCALIZED_RATE_BUTTON, APPIRATER_APP_NAME]

/*
 Text for button to remind the user to review later.
 */
#define APPIRATER_RATE_LATER			NSLocalizedStringFromTable(@"Remind me later", @"AppiraterLocalizable", nil)


@interface AppDelegate : UIResponder <UIApplicationDelegate,
                                      UIAlertViewDelegate,
                                      SKStoreProductViewControllerDelegate,
                                      GKLeaderboardViewControllerDelegate,
                                      GKAchievementViewControllerDelegate>{
    NSString* _appId;
    NSString* _templateReviewURL;
}

@property (strong, nonatomic) UIWindow *window;
@property (nonatomic) void *bgWorkerState;
@property(nonatomic, retain) id<GAITracker> tracker;

+ (natural_t) getFreeMemory;
+ (AppDelegate*)instance;

- (void)showRatingAlert;
- (void)startBgWorker:(void*)bws;
- (void)executeBgWork;
- (void)onBgWorkDone:(void*)obj;
- (void)submitScore:(long)score ForLeaderboard:(NSString*)leaderboard;
- (void)submitAchievement:(NSString*)achievement;
- (void)showLeaderboards;
- (void)showAchievements;
- (void)analyticsEventsWithCategory:(NSString*)category
                         withAction:(NSString*)action
                          withLabel:(NSString*)label
                          withValue:(NSNumber*)value;
- (void)analyticsScreen:(NSString*)screen;
- (void)postToTwitterScore:(long)score
                  withType:(int)type;
- (void)postToFacebookScore:(long)score
                  withType:(int)type;
- (void)postToGooglePlusScore:(long)score
                  withType:(int)type;
@end
