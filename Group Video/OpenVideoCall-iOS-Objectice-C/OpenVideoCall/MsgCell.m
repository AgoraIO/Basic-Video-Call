//
//  MsgCell.m
//  OpenVideoCall
//
//  Created by CavanSu on 24/11/2017.
//  Copyright © 2017 Agora. All rights reserved.
//

#import "MsgCell.h"

@interface MsgCell ()
@property (weak, nonatomic) IBOutlet UIView *backView;
@property (weak, nonatomic) IBOutlet UILabel *msgLabel;
@end

@implementation MsgCell

- (void)awakeFromNib {
    [super awakeFromNib];
    self.backgroundColor = [UIColor clearColor];
    self.contentView.backgroundColor = [UIColor clearColor];
    self.backView.layer.cornerRadius = 3;
    self.selectionStyle = UITableViewCellSelectionStyleNone;
}

- (void)setModel:(MsgModel *)model {
    
    self.backView.backgroundColor = model.color;
    self.msgLabel.text = model.message;
    [self layoutIfNeeded];
    
    if (!model.height) {
        model.height = CGRectGetMaxY(self.msgLabel.frame) + 5;
    }
}

@end
