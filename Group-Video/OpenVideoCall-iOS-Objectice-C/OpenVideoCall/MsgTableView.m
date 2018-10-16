//
//  MsgTableView.m
//  OpenVideoCall
//
//  Created by CavanSu on 24/11/2017.
//  Copyright © 2017 Agora. All rights reserved.
//

#import "MsgTableView.h"
#import "MsgCell.h"
#import "MsgModel.h"

@interface MsgTableView () <UITableViewDelegate, UITableViewDataSource>
@property (strong, nonatomic) NSMutableArray *msgArray;
@end

static NSString *cellID = @"cellID";

@implementation MsgTableView

- (void)awakeFromNib {
    [super awakeFromNib];
    self.delegate = self;
    self.dataSource = self;
    self.separatorStyle = UITableViewCellSeparatorStyleNone;
    self.backgroundColor = [UIColor clearColor];
}

#pragma mark- Append msg to tableView to display
- (void)appendMsgToTableViewWithMsg:(NSString *)message msgType:(MsgType)type
{
    MsgModel *model = [MsgModel modelWithMessage:message type:type];
    [self.msgArray insertObject:model atIndex:0];
    NSIndexPath *indexPath = [NSIndexPath indexPathForRow:0 inSection:0];
    [self insertRowsAtIndexPaths:@[indexPath] withRowAnimation:UITableViewRowAnimationLeft];
}

#pragma mark- <UITableViewDataSource>
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return self.msgArray.count;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    MsgCell *cell =  [tableView dequeueReusableCellWithIdentifier:cellID];
    MsgModel *model = self.msgArray[indexPath.row];
    cell.model = model;
    
    return cell;
}

#pragma mark- <UITableViewDelegate>
- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
    MsgModel *model = self.msgArray[indexPath.row];
    
    return model.height;
    
}
- (CGFloat)tableView:(UITableView *)tableView estimatedHeightForRowAtIndexPath:(NSIndexPath *)indexPath
{
    return 20;
}

- (NSMutableArray *)msgArray {
    
    if (!_msgArray) {
        _msgArray = [NSMutableArray array];
    }
    return _msgArray;
}

@end
