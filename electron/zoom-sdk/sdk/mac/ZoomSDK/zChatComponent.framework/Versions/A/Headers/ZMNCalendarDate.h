/*
 * ZMNCalendarDate.h
 */


/////////////////////////////////////////////
#define kCalendarDateStyle_HourMinutes                  1       //8:13 AM
#define kCalendarDateStyle_HourMinutes2                 2       //09:30 AM (AM always at right side)
#define kCalendarDateStyle_ShortRelativeTime            3       //Yesterday or 12:21 PM
#define kCalendarDateStyle_ShortRelativeDate            4       //Yesterday or 10/21/17
#define kCalendarDateStyle_ShortRelativeDateShortTime   5       //Yesterday, 11:30 AM or 10/16/20, 1:36 PM
#define kCalendarDateStyle_MediumRelativeDateShortTime  6       //Yesterday at 11:30 AM or Oct 16, 2020 at 1:36 PM

#define kCalendarDateNStyle_UseRelativeDate             (1 << 0)
#define kCalendarDateNStyle_TodayUseRelativeDate        (1 << 1)
#define kCalendarDateNStyle_TodayUseRelativeTime        (1 << 2)
#define kCalendarDateNStyle_WithWeekOfTime              (1 << 3)
#define kCalendarDateNStyle_WithWeekOfDate              (1 << 4)


/////////////////////////////////////////////
//EXTERN_C time_t mkdostime(unsigned dosDate, bool isdst);


/////////////////////////////////////////////ZMNCalendarDate
@interface ZMNCalendarDate : NSObject
{
    NSCalendar*                             calendar;
    NSDateFormatter*                        dateFormatter;
    
    NSDateFormatter*                        hourMinutesTime;
    NSDateFormatter*                        hourMinutesTime2;
    NSDateFormatter*                        shortRelativeTime;
    NSDateFormatter*                        shortRelativeDate;
    NSDateFormatter*                        shortRelativeDateShortTime;
    NSDateFormatter*                        mediumRelativeDateShortTime;
    
    NSDateFormatter*                        withinWeekTime;
    NSDateFormatter*                        withinYearTime;
    NSDateFormatter*                        aboveYearTime;
    NSDateFormatter*                        withinWeekDate;
    NSDateFormatter*                        withinYearDate;
    NSDateFormatter*                        aboveYearDate;
    NSDateFormatter*                        withinYearWeekTime;
    NSDateFormatter*                        aboveYearWeekTime;
    NSDateFormatter*                        withinYearWeekDate;
    NSDateFormatter*                        aboveYearWeekDate;
}

@property(nonatomic, readonly) NSCalendar* calendar;
@property(nonatomic, readonly) NSDateFormatter* dateFormatter;

@property(nonatomic, readonly) NSDateFormatter* hourMinutesTime;
@property(nonatomic, readonly) NSDateFormatter* hourMinutesTime2;
@property(nonatomic, readonly) NSDateFormatter* shortRelativeTime;
@property(nonatomic, readonly) NSDateFormatter* shortRelativeDate;
@property(nonatomic, readonly) NSDateFormatter* shortRelativeDateShortTime;
@property(nonatomic, readonly) NSDateFormatter* mediumRelativeDateShortTime;

@property(nonatomic, readonly) NSDateFormatter* withinWeekTime;
@property(nonatomic, readonly) NSDateFormatter* withinYearTime;
@property(nonatomic, readonly) NSDateFormatter* aboveYearTime;
@property(nonatomic, readonly) NSDateFormatter* withinWeekDate;
@property(nonatomic, readonly) NSDateFormatter* withinYearDate;
@property(nonatomic, readonly) NSDateFormatter* aboveYearDate;
@property(nonatomic, readonly) NSDateFormatter* withinYearWeekTime;
@property(nonatomic, readonly) NSDateFormatter* aboveYearWeekTime;
@property(nonatomic, readonly) NSDateFormatter* withinYearWeekDate;
@property(nonatomic, readonly) NSDateFormatter* aboveYearWeekDate;

/////////////////////////////////////////////
+ (ZMNCalendarDate*)shareInstance;
+ (void)releaseInstance;

- (BOOL)isDateInToday:(NSDate*)date;
- (BOOL)isDateInYesterday:(NSDate*)date;
- (BOOL)isDate:(NSDate*)date1 inSameDayAsDate:(NSDate*)date2;

//- (NSDate*)dateWithMSDOSFormat:(unsigned)dosDate isdst:(BOOL)isdst;

/////////////////////////////////////////////
- (NSString*)stringFromDate:(NSDate*)date withStyle:(unsigned)style;
- (NSString*)stringFromDate:(NSDate*)date withStyle:(unsigned)style timeZone:(NSTimeZone*)timeZone;

- (NSString*)getYYYYMMDDHHMMSSDescriptionFromDate:(NSDate*)date;

/********************************************
 * today: shortRelativeTime, [12:21 PM] or [13:13]
 *        shortRelativeDate(withStyle: kCalendarDateNStyle_TodayUseRelativeDate), [Today]
 *        shortRelativeDateShortTime(withStyle: kCalendarDateNStyle_TodayUseRelativeTime), [Today 12:21 PM]
 * yesterday/tomorrow: shortRelativeDateShortTime, [Yesterday 12:21 PM]
 *                     shortRelativeDate(withStyle: kCalendarDateNStyle_UseRelativeDate/kCalendarDateNStyle_WithWeekOfDate), [Yesterday]
 * same week: withinWeekTime, [Mon 12:21 PM]
 *            withinWeekDate(withStyle: kCalendarDateNStyle_UseRelativeDate/kCalendarDateNStyle_WithWeekOfDate), [Mon]
 * same year: withinYearTime, [21 Mar at 12:21 PM]
 *            withinYearDate(withStyle: kCalendarDateNStyle_UseRelativeDate), [21 Mar]
 *            withinYearWeekTime(withStyle: kCalendarDateNStyle_WithWeekOfTime), [Mon, 21 Mar at 12:21 PM]
 *            withinYearWeekDate(withStyle: kCalendarDateNStyle_WithWeekOfDate), [Mon, 21 Mar]
 * other: aboveYearTime, [11 Dec 2020 at 12:21 PM]
 *        aboveYearDate(withStyle: kCalendarDateNStyle_UseRelativeDate), [11 Dec 2020]
 *        aboveYearWeekTime(withStyle: kCalendarDateNStyle_WithWeekOfTime), [Mon, 11 Dec 2020 at 12:21 PM]
 *        aboveYearWeekDate(withStyle: kCalendarDateNStyle_WithWeekOfDate), [Mon, 11 Dec 2020]
 *******************************************/
- (NSString*)getNShortRelativeDescriptionFromDate:(NSDate*)date withStyle:(unsigned)style timeZone:(NSTimeZone*)timeZone;

@end

#define ZMNCALENDARDATE                     [ZMNCalendarDate shareInstance]
