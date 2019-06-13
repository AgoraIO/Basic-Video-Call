//
//  MsgCell.m
//  OpenVideoCall
//
//  Created by CavanSu on 24/11/2017.
//  Copyright © 2017 Agora. All rights reserved.
//

#import "MessageCell.h"
#import "CommonExtension.h"

@interface MessageCell ()
@property (weak, nonatomic) IBOutlet UIView *colorView;
@property (weak, nonatomic) IBOutlet UILabel *messageLabel;
@end

@implementation MessageCell
- (void)setMessage:(Message *)message {
    self.colorView.backgroundColor = (message.type == MessageTypeInfo ? UIColor.AGMessageInfoColor : UIColor.AGMessageErrorColor);
    self.messageLabel.text = message.text;
}
@end
