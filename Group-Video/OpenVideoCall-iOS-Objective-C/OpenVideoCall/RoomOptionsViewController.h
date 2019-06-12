//
//  RoomOptionsViewController.h
//  OpenVideoCall
//
//  Created by CavanSu on 2019/6/6.
//  Copyright © 2019 Agora. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface RoomOptions : NSObject
@property (assign, nonatomic) BOOL isDebugMode;
@end

@class RoomOptionsViewController;
@protocol RoomOptionsVCDelegate <NSObject>
- (void)roomOptions:(RoomOptionsViewController *)vc debugModeDidEnable:(BOOL)enable;
@end

@protocol RoomOptionsVCDataSource <NSObject>
- (RoomOptions *)roomOptionsVCNeedOptions;
@end

@interface RoomOptionsViewController : UITableViewController
@property (weak, nonatomic) id<RoomOptionsVCDataSource> dataSource;
@property (weak, nonatomic) id<RoomOptionsVCDelegate> delegate;
@end

