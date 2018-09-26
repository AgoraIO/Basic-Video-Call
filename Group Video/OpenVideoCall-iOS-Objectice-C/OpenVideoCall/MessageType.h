//
//  MessageType.h
//  OpenVideoCall
//
//  Created by CavanSu on 24/11/2017.
//  Copyright © 2017 Agora. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef NS_ENUM(int, MsgType) {
    MsgTypeChat = 0,
    MsgTypeError = 1
};

@interface MessageType : NSObject

@end
