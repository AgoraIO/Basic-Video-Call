//
//  MessageViewController.m
//  OpenVideoCall
//
//  Created by CavanSu on 2019/6/6.
//  Copyright © 2019 Agora. All rights reserved.
//

#import "MessageViewController.h"
#import "MessageCell.h"

@interface MessageViewController () <UITableViewDelegate, UITableViewDataSource>
@property (weak, nonatomic) IBOutlet UITableView *messageTableView;
@property (nonatomic, strong) NSMutableArray *list;
@end

@implementation MessageViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    self.messageTableView.rowHeight = UITableViewAutomaticDimension;
    self.messageTableView.estimatedRowHeight = 24;
}

- (void)appendInfo:(NSString *)text {
    Message *message = [Message messageWithText:text type:MessageTypeInfo];
    [self appendMessage:message];
}

- (void)appendError:(NSString *)text {
    Message *message = [Message messageWithText:text type:MessageTypeError];
    [self appendMessage:message];
}

#pragma mark - Private
- (void)appendMessage:(Message *)message {
    [self.list insertObject:message atIndex:0];
    NSIndexPath *indexPath = [NSIndexPath indexPathForRow:0 inSection:0];
    [self.messageTableView insertRowsAtIndexPaths:@[indexPath] withRowAnimation:UITableViewRowAnimationLeft];
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    return self.list.count;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    MessageCell *cell =  [tableView dequeueReusableCellWithIdentifier:@"MessageCell"];
    Message *message = self.list[indexPath.row];
    [cell setMessage:message];
    return cell;
}

- (NSMutableArray *)list {
    if (!_list) {
        _list = [NSMutableArray array];
    }
    return _list;
}
@end
