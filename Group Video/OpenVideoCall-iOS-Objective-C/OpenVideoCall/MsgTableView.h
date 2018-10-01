//
//  MsgTableView.h
//  OpenVideoCall
//
//  Created by CavanSu on 24/11/2017.
//  Copyright © 2017 Agora. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "MessageType.h"

@interface MsgTableView : UITableView

- (void)appendMsgToTableViewWithMsg:(NSString *)message msgType:(MsgType)type;

@end
