//
// Created by sy200 on 24-12-19.
//

#include "train_controller.h"
#include "train_model.h"
#include <string.h>

SearchResult
RailwaySystemSearchTrainData(TrainData* train_data, uint32_t num, uint32_t page_idx, const TrainQuery* search_request)
{
    SearchResult searchResult;
    getCount(search_request, &searchResult.data_total_count);
    getTrainList(search_request, train_data, num, page_idx, &searchResult.data_return_count);
    searchResult.page_count = searchResult.data_total_count / num;
    if(searchResult.data_total_count % num != 0)
    {
        searchResult.page_count += 1;
    }
    return searchResult;
}

int32_t
RailwaySystemInsertTrainData(TrainData train_data)
{
    return addTrain(&train_data) == 0 ? 0 : -1;
}

int32_t
RailwaySystemUpdateTrainData(TrainData train_data)
{
    TrainQuery query;
    memset(&query, 0, sizeof(TrainQuery));
    query.id       = train_data.id;
    query.query_id = true;

    TrainChange change;
    memset(&change, 0, sizeof(TrainChange));
    change.number         = train_data.number;
    change.start_station  = train_data.start_station;
    change.arrive_station = train_data.arrive_station;

    change.train_status  = train_data.train_status;
    change.ticket_price  = train_data.ticket_price;
    change.start_time    = train_data.start_time;
    change.arrive_time   = train_data.arrive_time;
    change.ticket_remain = train_data.ticket_remain;

    change.change_is_running     = true;
    change.change_id             = false;
    change.change_arrive_station = true;
    change.change_start_station  = true;
    change.change_arrive_time    = true;
    change.change_start_time     = true;
    change.change_number         = true;
    change.change_ticket_price   = true;
    change.change_ticket_remain  = true;

    return editTrain(&query, &change) == 0 ? 0 : -1;
}

int32_t
RailwaySystemDelTrainData(int32_t train_data_id)
{
    TrainQuery query;
    memset(&query, 0, sizeof(TrainQuery));
    query.id       = train_data_id;
    query.query_id = true;
    return deleteTrain(&query) == 0 ? 0 : -1;
}