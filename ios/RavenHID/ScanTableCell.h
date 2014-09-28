//
//  ScanTableCellTableViewCell.h
//  RavenHID
//
//  Created by lucas on 9/21/14.
//  Copyright (c) 2014 Lucas Morris. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface ScanTableCell : UITableViewCell

@property (strong, nonatomic) IBOutlet UILabel *NameLabel;
@property (strong, nonatomic) IBOutlet UILabel *RSSILabel;
@property (strong, nonatomic) IBOutlet UILabel *IdentifierLabel;

@end
