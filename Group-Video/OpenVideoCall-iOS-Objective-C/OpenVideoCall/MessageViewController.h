//
//  MessageViewController.h
//  OpenVideoCall
//
//  Created by CavanSu on 2019/6/6.
//  Copyright © 2019 Agora. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "Message.h"

@interface MessageViewController : UIViewController
- (void)appendInfo:(NSString *)text;
- (void)appendError:(NSString *)text;
@end
